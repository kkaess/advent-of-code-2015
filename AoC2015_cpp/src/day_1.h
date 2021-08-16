#include <string>
#include <iostream>

using namespace std;

void day_1(string s) {
	int sum = 0;

#pragma omp parallel for shared(s) reduction(+: sum)
	for (auto c : s) {
		if (c == '(')
			sum++;
		else if (c == ')')
			sum--;
	}

	cout << "Final Destination: " << sum << endl;

	int count = 0;
	for (decltype(s.length()) i = 0; i < s.length(); i++) {
		if (s[i] == '(')
			count++;
		else
			count--;
		if (count < 0) {
			cout << "Index of first visit to basement: " << i + 1 << endl;
			break;
		}
	}

	return;
}
