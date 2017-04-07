/*
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 *
 * ==========================================
 * @author Aryan Gupta
 * @date 2017-02-03 (YYYY-MM-DD)
 * @version 1.1
 * @project Birthday Paradox
 * @title Bithday Paradox Single Threaded Simulation
 * @description Runs a simulation finding the chance of 2 people
 *				having the birthday given number of people in a
 *				room (size of room)
 * ==========================================
 */
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const long TRAILS = 100000; // 1,000,000
const int PEOPLE = 75;

double pos[PEOPLE];

void simulate(int num) {
	int bdays[num];
	int matches = 0;
	
	for(int i = 0; i < TRAILS; ++i) {
		
		for(int a = 0; a < num; ++a) {
			bdays[a] = (rand() % 365) + 1;
		}
		
		bool sBreak = false;
		for(int a = 0; a < num; ++a) {
			for(int j = a + 1; j < num; ++j) {
				if(bdays[a] == bdays[j]) {
					matches++;
					sBreak = true;
					break;
				}
			}
			if(sBreak)
				break;
		}
	}
	
	pos[num - 1] = static_cast<double>(matches) / TRAILS;
}

int main() {
	srand(time(NULL));
	
	auto start = chrono::steady_clock::now();
	for(int i = 2; i <= PEOPLE; i++) {
		simulate(i);
	}
	
	auto diff = chrono::steady_clock::now() - start;
	double nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	for(int i = 1; i < PEOPLE; i++)
		cout << i + 1 << " :: " << pos[i] << endl;

	cout << "TIME TO EXECUTE:: " << nseconds << endl;
	return 0;
}