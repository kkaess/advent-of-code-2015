#include <string>
#include <regex>
#include <fstream>
#include <iostream>

using namespace std;
enum instruction {
	on, off, toggle, err
};
struct direction {
	instruction instr;
	size_t x_lb;
	size_t x_ub;
	size_t y_lb;
	size_t y_ub;
};

instruction get_instruction(const string &s) {
	static const string turn_off { "turn off" };
	static const string turn_on { "turn on" };
	static const string toggle { "toggle" };
	if (s.compare(turn_off) == 0)
		return instruction::off;
	if (s.compare(turn_on) == 0)
		return instruction::on;
	if (s.compare(toggle) == 0)
		return instruction::toggle;
	return instruction::err;
}

direction parse_direction(const string &s) {
	const static regex direction_match {
			"([a-z ]+) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)" };
	smatch m;
	direction d;
	regex_search(s, m, direction_match, regex_constants::match_any);

	d.instr = get_instruction(m[1].str());
	d.x_lb = (size_t) stoi(m[2].str());
	d.x_ub = (size_t) stoi(m[4].str());
	d.y_lb = (size_t) stoi(m[3].str());
	d.y_ub = (size_t) stoi(m[5].str());
	return d;
}

void day_6(string filename) {
	ifstream file;
	file.open(filename);
	vector<string> s { };
	char line[100];
	while (file.getline(line, 100).good()) {
		s.push_back(string(line));
	}
	file.close();

	vector<direction> directions { s.size() };
#pragma omp parallel for
	for (size_t i = 0; i < s.size(); i++) {
		directions[i] = parse_direction(s[i]);
	}

	bool light_grid[1000][1000];
#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < 1000; i++) {
		for (size_t j = 0; j < 1000; j++) {
			light_grid[i][j] = false;
		}
	}

	for (auto direction : directions) {
#pragma omp parallel for collapse(2)
		for (size_t i = direction.x_lb; i <= direction.x_ub; i++) {
			for (size_t j = direction.y_lb; j <= direction.y_ub; j++) {
				switch (direction.instr) {
				case instruction::off:
					light_grid[i][j] = false;
					break;
				case instruction::on:
					light_grid[i][j] = true;
					break;
				case instruction::toggle:
					light_grid[i][j] = !(light_grid[i][j]);
					break;
				default:
					cout << "Danger, Will Robinson!" << endl;
				}
			}
		}
	}
	int total_lit = 0;
#pragma omp parallel for collapse(2) reduction(+:total_lit)
	for (size_t i = 0; i < 1000; i++) {
		for (size_t j = 0; j < 1000; j++) {
			if (light_grid[i][j])
				total_lit++;
		}
	}
	cout << "Total number of lights lit: " << total_lit << endl;

	int new_grid[1000][1000];

#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < 1000; i++) {
		for (size_t j = 0; j < 1000; j++) {
			new_grid[i][j] = 0;
		}
	}

	for (auto direction : directions) {
#pragma omp parallel for collapse(2)
		for (size_t i = direction.x_lb; i <= direction.x_ub; i++) {
			for (size_t j = direction.y_lb; j <= direction.y_ub; j++) {
				switch (direction.instr) {
				case instruction::off:
					int diff;
					if ((diff = (new_grid[i][j]-1)) < 0) {
						new_grid[i][j] = 0;
					} else {
						new_grid[i][j] = diff;
					}
					break;
				case instruction::on:
					new_grid[i][j] += 1;
					break;
				case instruction::toggle:
					new_grid[i][j] += 2;
					break;
				default:
					cout << "Danger, Will Robinson!" << endl;
				}
			}
		}
	}
	int total_brightness = 0;
#pragma omp parallel for collapse(2) reduction(+:total_brightness)
	for (size_t i = 0; i < 1000; i++) {
		for (size_t j = 0; j < 1000; j++) {
			total_brightness += new_grid[i][j];
		}
	}
	cout << "Total brightness with correct understanding: " << total_brightness << endl;
}
