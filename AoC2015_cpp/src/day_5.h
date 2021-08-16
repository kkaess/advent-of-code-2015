#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <iostream>

void day_5(string filename) {
	ifstream file;
	file.open(filename);
	vector < string > s { };
	char line[100];
	while (file.getline(line, 100).good()) {
		s.push_back(string(line));
	}
	file.close();

	regex three_vowels { "(?:[aeiou][^aeiou]*?){3}" };
	regex doubles { "(.)\\1" };
	regex bad_combos { "ab|cd|pq|xy" };

	regex double_doubles { "(.{2}).*\\1" };
	regex double_single { "(.).\\1" };

	int good_strings = 0;
	int new_good_strings = 0;
#pragma omp parallel for reduction(+:good_strings,new_good_strings)
	for (auto str : s) {
		if (regex_search(str, three_vowels, regex_constants::match_any)
				&& regex_search(str, doubles, regex_constants::match_any)
				&& (!regex_search(str, bad_combos, regex_constants::match_any)))
			good_strings++;
		if (regex_search(str, double_doubles, regex_constants::match_any)
				&& regex_search(str, double_single, regex_constants::match_any))
			new_good_strings++;
	}

	cout << "Number of good strings: " << good_strings << endl;

	cout << "Number of good strings under new rules: " << new_good_strings
			<< endl;

	return;

}
