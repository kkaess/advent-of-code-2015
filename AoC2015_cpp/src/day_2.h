#include <iostream>
#include <fstream>
#include <omp.h>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;

void day_2(string filename) {

	ifstream file;
	file.open(filename);
	vector<string> s { };
	char line[20];
	while (file.getline(line, 20).good()) {
		s.push_back(string(line));
	}
	file.close();

	auto size = s.size();
	vector<string> strs;
	vector<vector<int>> dims { size };
#pragma omp parallel for shared(dims,s) private(strs)

	for (decltype(size) i = 0; i < size; i++) {
		boost::split(strs, s[i], boost::is_any_of("x"));
		for (auto str : strs) {
			dims[i].push_back(stoi(str));
		}
		sort(dims[i].begin(), dims[i].end());
	}

	long wrapping_paper = 0;
	long ribbon = 0;

#pragma omp parallel for shared(dims) reduction(+:wrapping_paper,ribbon)
	for (auto dim : dims) {
		wrapping_paper += 3 * dim[0] * dim[1]
				+ 2 * (dim[1] * dim[2] + dim[0] * dim[2]);
		ribbon += (2 * (dim[0] + dim[1]) + dim[0] * dim[1] * dim[2]);
	}

	cout << "Size of Wrapping Paper: " << wrapping_paper << endl;
	cout << "Length of Ribbon: " << ribbon << endl;

	return;
}
