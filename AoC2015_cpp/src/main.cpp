/*
 * main.cpp
 *
 *  Created on: Feb 22, 2021
 *      Author: kaess
 */
#include <iostream>
#include <string>
#include "problems.h"
using namespace std;

int main(int argc, char **argv) {
	int day = atoi(argv[1]);

	switch (day) {
	case 1:
		day_1(string(argv[2]));
		break;
	case 2:
		day_2(string(argv[2]));
		break;
	case 3:
		day_3(string(argv[2]));
		break;
	case 4:
		day_4(string(argv[2]));
		break;
	case 5:
		day_5(string(argv[2]));
		break;
	case 6:
		day_6(string(argv[2]));
		break;
	case 7:
		day_7(string(argv[2]));
		break;
	default:
		cout << "No Day Selected." << endl;
	}

	return 0;
}

