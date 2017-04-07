/*
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 *
 * ==========================================
 * @author Aryan Gupta
 * @date 2017-02-02 (YYYY-MM-DD)
 * @version 1.0
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
#include <thread>


using namespace std;

const unsigned long long NO_OF_TESTS = 100000000000000000; //10,000,000,000,000,000
const int NUM_OF_PEOPLE = 370;
double possibility[NUM_OF_PEOPLE];

int createBday() {return (rand() % 365) + 1;}

void simulate(int num) {
	int bdays[num];
	unsigned long long matches = 0;
	
	for(unsigned long long i = 0; i < NO_OF_TESTS; ++i) {

		for(int a = 0; a < num; ++a) {
			bdays[a] = createBday();
			//cout << bdays[a] << endl;
		}
 /* USING GOTO		
		for(int a = 0; a < num; ++a)
			for(int j = a + 1; j < num; ++j)
				if(bdays[a] == bdays[j]) {
					matches++;
					goto A;
				}
		A: ;
// */
		
// /* // USING FLAG VARIABLE
				
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
// */
	}
	
	possibility[num] = static_cast<double>(matches) / NO_OF_TESTS;
}

int main() {
	srand(time(NULL));
	
	thread t[NUM_OF_PEOPLE];
	
	for(int i = 1; i <= NUM_OF_PEOPLE; i++) {
		t[i] = thread(simulate(i));
	}
	
	for(auto& i: t)
		i.join();
	
	
	for(int i = 2; i <= NUM_OF_PEOPLE; i++) {
		cout << "For " << i << " people, the probibility of two birthdays is about " << possibility[i] << endl;
	}
	
	return 0;
}

/*
/*

*
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <thread>
#include <mutex> 
#include <vector>

using namespace std;

const int NO_OF_TESTS = 5000;
//const unsigned long long NO_OF_TESTS = 100000000000000000; //10,000,000,000,000,000
const int NUM_OF_PEOPLE = 20;
//vector<double> possibility(NUM_OF_PEOPLE, 0);
double possibility[NUM_OF_PEOPLE];
mutex mtx;

int createBday() {return (rand() % 365) + 1;}

void simulate(int num) {
	mtx.lock();
	int bdays[num];
	//unsigned long long matches = 0;
	int matches = 0;

	for(int i = 0; i < NO_OF_TESTS; ++i) {	

		for(int a = 0; a < num; ++a) {
			bdays[a] = createBday();
			//cout << bdays[a] << endl;
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

	cout << "\t\t Vars" << matches << "| " << NO_OF_TESTS << endl;
	possibility[num] = static_cast<double>(matches) / NO_OF_TESTS;
	mtx.unlock();
}

int main() {
	srand(time(NULL));
	
	vector<thread> t;
	t.reserve(NUM_OF_PEOPLE);
	
	for(int i = 2; i <= NUM_OF_PEOPLE; i++) {
		//cout << "For " << i << " people, the probibility of two birthdays is about " << simulate(i) << endl;
		t[i] = thread(simulate, i);
	}
	
	for(auto& i: t)
		i.join();
	
	for(int i = 2; i <= NUM_OF_PEOPLE; i++) {
		cout << "For " << i << " people, the probibility of two birthdays is about " << possibility[i] << endl;
	}
	
	return 0;
}
*/