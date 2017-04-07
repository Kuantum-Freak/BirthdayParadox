/* 
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 *
 * ====================================================================================
 * @author Aryan Gupta
 * @project Bithday Paradox
 * @title Birthday Paradox using Different Thread Models
 * @date 2017-02-11 (YYYY-MM-DD)
 * @fversion 4.3
 * @description Runs a simulation finding the chance of 2 people
 *				having the birthday given number of people in a
 *				room (size of room)
 * ====================================================================================
 */
 
#include "info.h"
#define _PRINTINFO std::cout << '\n' << R_PROJNAME << " v" << R_PVERSION << " by " << R_AUTHOR << '\n' << R_COPYRIGHT << '\n' << R_COMMENTS << "\n\n\n" // Legal and Informational

#include <iostream>
#include <vector>
#include <chrono>

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
#elif (defined (_WIN32) || defined (_WIN64))
	#include ".\h\mingw.thread.h"
	#include ".\h\mingw.mutex.h"
#endif

using std::cout;
using std::endl;
using std::vector;
using std::thread;

typedef unsigned usig;
typedef unsigned long long ull;

usig NUM_OF_THREADS = 4; // stores the amount of threads to use

ull NO_OF_TESTS = 5000; //stores the amount of simulations to run
usig NUM_OF_PEOPLE = 25; // stores the max number of people that will be in a room
bool display = false; // display full calculated data

vector<ull> matches;
ull matches_singleNum = 0; // stores the all the matches | USED ONLY DURING SIGLE RUN
vector<ull> numbersRan(1, 1); // stores the room simulations allready done, used for thread pool so threads knows which next simulation to run

std::mutex sims; // mutx used when thread gets number to simulate
std::mutex poss; // mutex used when writing the number of matches found after running sumulation
std::mutex smat; // mutex used when updateing number of matches in single runs

/* 
	@summary 	Struct hold a double. it allows decimal places far beyond a normal double can hole
 */
struct Double {
	vector<usig> wholePart; // stores the integer part
	vector<usig> decimalPart; // stpres the decimal part
	void print() { // prints it out in a proper format
		using std::cout;
		for(usig i = 0; i < wholePart.size(); ++i)
			cout << wholePart[i];
		cout << ".";
		for(usig i = 0; i < decimalPart.size(); ++i)
			cout << decimalPart[i];
	}
};

/* 
	@summary 	Used to return parse options
 */
struct ParseInfo {
	bool runSpecificNum = false; // stores if the program should run a speific number
	ull numToRun = 0; // if previous bool is true, then this holds the number to test
};

/* 
	@summary 	this function does long division. Used because normal division 
				operator does cannot divide to large number of decimal places
	
	@param		2 unsinged lng variables will be the dividor and dividen (a / b)
				and n is the number of decimal places to go to
	@return		returns a Decimal struct that is the answer to a/b
 */
Double divide(ull a, ull b, int n) { // devides two numbers, used n number of decimal places. used Double struct to store data
	Double number; // create holding variable
	number.wholePart.push_back(a / b); // integer division (simplified here)
	
	ull c = a % b;
	for(int i = 0; i < n; i++) { // decimal division (LONG DIVISION)
		if(c < b)
			c *= 10;
		number.decimalPart.push_back(c / b);
		c = c % b;
		if(c == 0)
			break;
	}
	return number;
}

/* 
	@summary	runs a simulation. The simulation sets up rooms with num amount of people.
				It gives each person in the room a birthday defined by a number frim 1 to 365
				Then it goes in each room and sees if there is 2 people with matching birthdays
				If there is then it increments matches. It goes through all the rooms
				If we are running singles then it sets up the rooms but only goes to room
				number start to room number end, and leaves the rest to the other threads
	
	@param		1: number of people in the room to simulate
				2: whether we are running singles or all
				3: used when running singles, defines start of simulation number
				4: used when running singles, defines end of simulation number
	@return		In case of running all of the simulation, it returns -1
				In case of running singles it returns the amount of matches the simulation found
 */
ull simulate(ull num, bool single, ull start = 0, ull end = NO_OF_TESTS) { // simulates a number of people in a room
	int bdays[num]; // stores all the birhdays  of the people in a room
	ull matches = 0; // stores all the matches
	
	for(ull i = start; i < end; ++i) { 				// go through each room that the simulation is assigned to 
		for(ull a = 0; a < num; ++a)   				// give each person a birthday
			bdays[a] = (rand() % 365) + 1;
		
		bool sBreak = false; 										// used so we can beak out of 2 loops at once
		for(ull a = 0; a < num; ++a) {				// select person 1
			for(unsigned long  long j = a + 1; j < num; ++j) {		// select person 2
				if(bdays[a] == bdays[j]) {							// see if they have matching birthdays
					matches++;										// if they do then increament matches
					sBreak = true;									// and break out of two loops
					break;
				}													// If they dont then select the next person. See
			}														// handshake problem for more info on this algoritm
			if(sBreak)
				break;
		}
	}
	if(single) { // if we are running singles then add the amount of matches this thread found to the global var
		smat.lock();
		matches_singleNum += matches;
		smat.unlock();
	} else // if we are running all simulations then just return the amount of matches
		return matches;
	return -1;
}

/* 
	@summary	Starts the threadpool. Picks the last number from the
				numbersRan vector and adds one to it. this will ensure
				that the number hasnt been run before. Then the thread
				runs a simulation with that many number of people. after
				getting the result updates the matches vector with the
				simulation data (number of matches). If the nnumber it
				gets from the numbersRan exceeds the end number then we
				have finished all the simulations so we exit the while loop
				
	@param		VOID
	@return		VOID
 */
void threadPool() {
	while(true) {
		sims.lock();
		ull people = numbersRan.back() + 1;		// get last number ran and add one, this will be the simulation this thread will run
		numbersRan.push_back(people);							// put number back so next thread knows to simulate the next num
		sims.unlock();
		
		if(people > NUM_OF_PEOPLE) // if the number exeeds the last simulation we need to do, end thread
			break;
		
		ull numMatch = simulate(people, false); // run simulations and get number of matches

		poss.lock();
		matches[people] = numMatch; // update variable with the number of matches we found
		poss.unlock();
	}
}

/* 
	@summary	Creates threads if we are running all simulation rooms
	
	@param		Reference of the vector holding all the threads
	@return		VOID
	
 */
void startThreads(vector<thread>& threads) { // start entire pradox
	for(usig i = 0; i < NUM_OF_THREADS; i++)
		threads.push_back(thread(threadPool));
	
	for(auto& i: threads) i.join();
}

/* 
	@summary	Creates threads if we are running one room simulation
	
	@param		1: Reference of the vector holding all the threads
				2: The number of people in the room when running single simulations
	@return		VOID
 */
void startSingles(vector<thread>& threads, const ull num) { // start single num test
	ull distance = NO_OF_TESTS / NUM_OF_THREADS; // divide rooms in to equal chunks
	
	for(usig i = 0; i < NUM_OF_THREADS - 1; i++)
		threads.push_back(thread(simulate, num, true, (distance * i), (distance * (i + 1))));
	threads.push_back(thread(simulate, num, true, (distance * (NUM_OF_THREADS - 1)), NO_OF_TESTS));
	/* // THIS WAS THE OLD WAY - creates a one off error (only runs end - 1 simulations)
	for(usig i = 0; i < NUM_OF_THREADS; i++) // run simulations with each thread taking a chunk
		threads.push_back(thread(simulate, num, true, (distance * i), (distance * (i + 1))));
	//*/
	for(auto& i: threads) i.join(); // wait for them to finish
}

/* 
	@summary 	displays help text
	
	@param		VOID
	@return		VOID
 */
void helpText() { // display help text
	using std::cout;
	using std::endl;
	cout << "Birthday Paradox is a simulation program that simulates the birthday paradox" << endl << endl;
	cout << "HELP TEXT:\n" << "Parameter\tDefault\t\tDetails" << endl;
	cout << "-t \t\t" << NUM_OF_THREADS              << "\t\tUse this argument to specify number of threads\n"
		 << "-s \t\t" << NO_OF_TESTS                 << "\t\tUse this argument to specify the number of simulations\n"
		 << "-p \t\t" << NUM_OF_PEOPLE               << "\t\tUse this argument to specify the maximum number of people\n"
		 << "-r \t\t" << "No Default"                << "\tUse this argument to specify the number of people, runs singles\n"
		 << "-d \t\t" << ((display)? "True":"False") << "\t\tUse this argument to specify whether to display results\n"
		 << endl;
	
	cout << "Usage:   .\\BirthdayPDX.exe [-t <threads>] [-s <simulations>] [-p <people>] [-d] [-r <people>]" << endl;
	cout << "Example: .\\BirthdayPDX.exe -t 7 -s 500000 -p 50" << endl;
	cout << "Example: .\\BirthdayPDX.exe -t 7 -s 500000 -d -r 50" << endl;
	
	cout << endl << endl;
	
	exit(1);
}

/* 
	@summary	Parses the command line arguments
	
	@param		An int containing the amount of arguments supplied, and a char
				array holding the actual command line arguments
	@return		A struct holding a boolean (if we should run a single simulation)
				and the number we should run it on in case we are running singles
 */
ParseInfo parse(int argc, char* argv[]) { // parse command line arguments
	ParseInfo info;
	for(int i = 1; i < argc; ++i) {
		if       (strcmp(argv[i], "-t") == 0) { // number of threads to use
			NUM_OF_THREADS = atoi(argv[i + 1]);
			i++;
		} else if(strcmp(argv[i], "-s") == 0) { // number of tests/rooms to create
			NO_OF_TESTS = atol(argv[i + 1]);
			i++;
		} else if(strcmp(argv[i], "-p") == 0) { // max number of people in a room when simulating
			NUM_OF_PEOPLE = atoi(argv[i + 1]);
			i++;
		} else if(strcmp(argv[i], "-r") == 0) { // run single number
			info.runSpecificNum = true;
			info.numToRun = static_cast<ull>(atol(argv[i + 1]));
			i++;
		} else if(strcmp(argv[i], "-d") == 0) { // display run data
			display = true;
		} else if(strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-?") == 0 ) // user needs help, display help text
			helpText();
		else // somthing went wrong, display help text
			helpText();
	}
	return info;
}

/* 
	@summary	MAIN
	
	@param		Command line arguments
	@return		exit code
 */
int main(int argc, char* argv[]) {
	_PRINTINFO;
	
	using namespace std::chrono;
	srand(time(NULL));
	
	ParseInfo info = parse(argc, argv); // get parse info
	matches.resize(NUM_OF_PEOPLE + 2); // reallocate memory for faster computation
	
	vector<thread> threads; // holds all threads
	
	if(info.runSpecificNum) { // run singles
		auto start = steady_clock::now();
		startSingles(threads, info.numToRun); // RUN
		auto end = steady_clock::now();
		
		auto diff = end - start;
		auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den; // get how long it took
		
		if(display) { // if we want to display the run data
			cout << ((info.numToRun < 100)? (info.numToRun < 10)? "For   ":"For  ":"For ") << info.numToRun << " people, the probibility of two birthdays is about ";
			divide(matches_singleNum, NO_OF_TESTS, 30).print();
			cout << endl;
		}
		
		cout << "Running the Bithday Paradox Sim using " << NUM_OF_THREADS << " threads and " << NO_OF_TESTS << " tests for " << info.numToRun << " people took " << nseconds << " seconds\n\n"; // display the diagnostics
	} else { // run all simulations
		auto start = steady_clock::now();
		startThreads(threads); // RUN
		auto end = steady_clock::now();
		
		auto diff = end - start;
		auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den; // get how long it took
		
		if(display) { // if we want to display the run data
			for(usig i = 2; i <= NUM_OF_PEOPLE; i++) { // go through each simulation
				cout << ((i < 100)? (i < 10)? "For   ":"For  ":"For ") << i << " people, the probibility of two birthdays is about ";
				divide(matches[i], NO_OF_TESTS, 5).print();
				cout << endl;
			}
		}
		
		cout << "Running the Bithday Paradox Sim using " << NUM_OF_THREADS << " threads and " << NO_OF_TESTS << " tests using a max of " << NUM_OF_PEOPLE << " people took " << nseconds << " seconds\n"; // display the diagnostics
	}
	
	std::cin.get();
	
	return 0; // SUCCESS
}

