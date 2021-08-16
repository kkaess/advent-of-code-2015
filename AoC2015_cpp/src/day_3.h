/*
 * day_3.h
 *
 *  Created on: Feb 25, 2021
 *      Author: kaess
 */

#include <iostream>
#include <fstream>
#include <omp.h>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;

// Resets pointer to start of stream... just as a heads up.
size_t file_length(ifstream &ifs) {
	ifs.seekg(0, ifs.end);
	size_t length = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	return length;
}

vector<int> grid_lims(vector<char> &instructions) {
	int x = 0, y = 0, min_x = 0, max_x = 0, min_y = 0, max_y = 0;

	for (auto c : instructions) {
		switch (c) {
		case '>':
			x++;
			if (x > max_x)
				max_x = x;
			break;
		case '<':
			x--;
			if (x < min_x)
				min_x = x;
			break;
		case '^':
			y++;
			if (y > max_y)
				max_y = y;
			break;
		case 'v':
			y--;
			if (y < min_y)
				min_y = y;
		}
	}
	return {min_x,max_x,min_y,max_y};
}

void fill_grid(vector<vector<int>> &grid, const vector<char> &instructions,
		int x_start, int y_start) {
	int x = x_start, y = y_start;
	grid[x][y] += 1;
	for (auto c : instructions) {
		switch (c) {
		case '>':
			x++;
			break;
		case '<':
			x--;
			break;
		case '^':
			y++;
			break;
		case 'v':
			y--;
			break;
		}
		grid[x][y] += 1;
	}
	return;
}

int count_grid(const vector<vector<int>> &grid) {
	const auto size_x = grid.size();
	const auto size_y = grid[0].size();
	int sum = 0;
#pragma omp parallel for collapse(2) reduction(+:sum)
	for (size_t i = 0; i < size_x; i++) {
		for (size_t j = 0; j < size_y; j++) {
			if (grid[i][j])
				sum += 1;
		}
	}
	return sum;
}

void day_3(string filename) {

	ifstream file { filename };
	auto length = file_length(file);
	char line[length];
	file.get(line, length);
	file.close();
	vector<char> instructions;
	vector<char> santas_instructions;
	vector<char> robo_santas_instructions;

	for (decltype(length) i = 0; i < length; i++) {
		instructions.push_back(line[i]);
		if (i % 2)
			robo_santas_instructions.push_back(line[i]);
		else
			santas_instructions.push_back(line[i]);
	}

	auto lims = grid_lims(instructions);
	auto x_start = -lims[0];
	auto y_start = -lims[2];
// Narrowing conversion, noted...
	size_t size_x { size_t(lims[1] - lims[0] + 1) };
	size_t size_y { size_t(lims[3] - lims[2] + 1) };

	vector<vector<int>> grid(size_x);
#pragma omp parallel for
	for (size_t i = 0; i < size_x; i++) {
		grid[i].resize(size_y, 0);
	}

	fill_grid(grid, instructions, x_start, y_start);

	cout << "Number of houses visited: " << count_grid(grid) << endl;

	auto lims_s = grid_lims(santas_instructions);
	auto lims_r = grid_lims(robo_santas_instructions);

	lims[0] = (lims_s[0]<lims_r[0]?lims_s[0]:lims_r[0]);
	lims[1] = (lims_s[1]>lims_r[1]?lims_s[1]:lims_r[1]);
	lims[2] = (lims_s[2]<lims_r[2]?lims_s[2]:lims_r[2]);
	lims[3] = (lims_s[3]>lims_r[3]?lims_s[3]:lims_r[3]);

	x_start = -lims[0];
	y_start = -lims[2];
	size_x = lims[1]-lims[0]+1;
	size_y = lims[3]-lims[2]+1;
	grid.resize(size_x);
	for (size_t i = 0; i < size_x; i++) {
		grid[i].clear();
		grid[i].resize(size_y,0);
	}

	fill_grid(grid,santas_instructions,x_start,y_start);
	fill_grid(grid,robo_santas_instructions,x_start,y_start);

	cout << "Number of houses visited next year: " << count_grid(grid) << endl;

	return;
}

