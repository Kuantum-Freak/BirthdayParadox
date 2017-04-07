/*
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 *
 * ==========================================
 * @author Aryan Gupta
 * @date 2017-02-06 (YYYY-MM-DD)
 * @version 3.0
 * @project Birthday Paradox
 * @title Bithday Paradox Using Both Excessive Threads and Single Threads
 * @description Runs a simulation finding the chance of 2 people
 *				having the birthday given number of people in a
 *				room (size of room)
 * ==========================================
 */
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>

// MT
#include <thread>
#include <mutex>
// END

using namespace std;
using namespace std::chrono;

const long TRAILS = 1000000; // 1,000,000
const int PEOPLE = 370;

double pos_MT[PEOPLE];
double pos_ST[PEOPLE];

// MT
mutex mtx;
// END

void simulate_MT(int num) {
	int bdays[num];
	int matches = 0;

	// MT
	//mtx.lock();
	//long trails = TRAILS; //copy const var locally
	//mtx.unlock();
	// END
	
	for(int i = 0; i < TRAILS; ++i) {
		
		for(int a = 0; a < num; ++a) {
			// MT
			mtx.lock();
			// END
			
			bdays[a] = (rand() % 365) + 1;

			// MT
			mtx.unlock();
			// END
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
	
	// MT
	mtx.lock();
	// END

	pos_MT[num - 1] = static_cast<double>(matches) / TRAILS;
	// cout << "MT: " << num << " end\n";
	//MT
	mtx.unlock();
	// END
}

void simulate_ST(int num) {
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
	
	pos_ST[num - 1] = static_cast<double>(matches) / TRAILS;
}

void runThreadedSimulation() {
	
	cout << "Threaded Simulation: " << endl;
	
	// MT
	vector<thread> threads;
	//threads.reserve(PEOPLE);
	//thread t[PEOPLE];
	// END

	auto start = chrono::steady_clock::now();
	for(int i = 2; i <= PEOPLE; i++) {
		// MT
		threads.push_back(thread(simulate_MT, i));
		// END
	}
	
	// MT
	for(auto& i: threads) i.join();
	// END
	
	auto diff = chrono::steady_clock::now() - start;
	double nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	for(int i = 1; i < PEOPLE; i++)
		cout << i + 1 << " :: " << pos_MT[i] << endl;

	cout << "TIME TO EXECUTE:: " << nseconds << endl;
}

void runSingleSimulation() {
	cout << "Single Simulation: " << endl;
	
	start = chrono::steady_clock::now();
	for(int i = 2; i <= PEOPLE; i++) {
		simulate_ST(i);
	}
	
	diff = chrono::steady_clock::now() - start;
	nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	for(int i = 1; i < PEOPLE; i++)
		cout << i + 1 << " :: " << pos_ST[i] << endl;

	cout << "TIME TO EXECUTE:: " << nseconds << endl;
}

int main() {
	srand(time(NULL));
	
	runThreadedSimulation();
	cout << endl << endl << endl;
	runSingleSimulation();
	
	return 0;
}