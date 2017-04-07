/*
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 *
 * ==========================================
 * @author Aryan Gupta
 * @date 2017-02-05 (YYYY-MM-DD)
 * @version 2.1
 * @project Birthday Paradox
 * @title Bithday Paradox Object-Oriented Multi-Threaded Simulation
 * @description Runs a simulation finding the chance of 2 people
 *				having the birthday given number of people in a
 *				room (size of room)
 * ==========================================
 */
// https://www.tutorialcup.com/cplusplus/multithreading.htm
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <thread>
//#include <mutex> 
#include <vector>

using namespace std;

const int NO_OF_TESTS = 5000;
//const unsigned long long NO_OF_TESTS = 100000000000000000; //10,000,000,000,000,000
const int NUM_OF_PEOPLE = 20;
//vector<double> possibility(NUM_OF_PEOPLE, 0);
//double possibility[NUM_OF_PEOPLE];
//mutex mtx;

class Room {
    public:
        Room(int people);
        void simulate();
        double getPossibility();
    private:
        int createBDay();
        void test();

        int people;
        int matches;
		double possibility;
};

Room::Room(int people) {
	this->people = people;
	this->simulate();
}

int Room::createBDay() { return (rand() % 365) + 1; }

void Room::test() {
	int bdays[this->people];
	for(int a = 0; a < people; ++a) {
		bdays[a] = createBDay();
		//cout << bdays[a] << endl;
	}

 /* USING GOTO	
	for(int a = 0; a < num; ++a)
		for(int j = a + 1; j < num; ++j)
			if(bdays[a] == bdays[j]) {
				this->matches++;
				goto A;
			}
	A: ;

// */	
// /* // USING FLAG VARIABLE
			
	bool sBreak = false;
	for(int a = 0; a < people; ++a) {
		for(int j = a + 1; j < people; ++j) {
			if(bdays[a] == bdays[j]) {
				this->matches++;
				sBreak = true;
				break;
			}
		}
		if(sBreak)
			break;
	}
// */
}

void Room::simulate() {
	for(int i = 0; i < NO_OF_TESTS; ++i) {
		test();
	}
	
	this->possibility = static_cast<double>(this->matches) / NO_OF_TESTS;
}

double Room::getPossibility() {
	return this->possibility;
}

int main() {
	srand(time(NULL));
	
	vector<thread> t;
	t.reserve(NUM_OF_PEOPLE);
	vector<Room> r;
	r.reserve(NUM_OF_PEOPLE);
	
	for(int i = 1; i < NUM_OF_PEOPLE; i++) {
		r[i] = Room(i + 1);
		t[i] = thread(&Room:simulate, &r[i]);
	}
	
	for(auto& i: t)
		i.join();
	
	for(int i = 1; i < NUM_OF_PEOPLE; i++) {
		cout << "For " << (i + 1) << " people, the probibility of two birthdays is about " << r[i].getPossibility() << endl;
	}
	
	return 0;
}