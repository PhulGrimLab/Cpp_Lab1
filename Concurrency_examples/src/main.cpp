/*
	ch01_mt_example.cpp - main file for the Chapter 01 multithreaded example.
	
	Revision 0
	
	Features:
			- 
			
	Notes:
			- 
			
	2016/10/30, Maya Posch
*/


#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

using namespace std;

#ifndef THREADFNC_MODE
#define THREADFNC_MODE 0
#endif

struct vecValue {
	int tid = 0;
	int value = 0;
};


// --- Globals
mutex values_mtx;
mutex cout_mtx;
mutex threadFnc_mtx;
vector<vecValue> values;


int randGen(const int& min, const int& max) {
    static thread_local mt19937 generator(hash<thread::id>()(this_thread::get_id()));
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}


void threadFncParallel(int tid) {
	vecValue val;
	val.tid = tid;

	{
		lock_guard<mutex> lock(cout_mtx);
		cout << "~~Starting thread " << tid << "~~\n";
	}

	{
		lock_guard<mutex> lock(values_mtx);
		val.value = values[0].value;
	}

	int rval = randGen(0, 10);
	val.value += rval;

	{
		lock_guard<mutex> lock(cout_mtx);
		cout << "Thread " << tid << " adding " << rval << ". New value: " << val.value << ".\n";
	}

	{
		lock_guard<mutex> lock(values_mtx);
		values.push_back(val);
	}
}


void threadFncSerialized(int tid) {
	lock_guard<mutex> fn_lock(threadFnc_mtx);

	cout << "~~Starting thread " << tid << "~~\n";
	vecValue val;
	val.tid = tid;
	val.value = values[0].value;
	int rval = randGen(0, 10);
	val.value += rval;
	cout << "Thread " << tid << " adding " << rval << ". New value: " << val.value << ".\n";
	values.push_back(val);
}


void threadFnc(int tid) {
#if THREADFNC_MODE == 0
	threadFncParallel(tid);
#elif THREADFNC_MODE == 1
	threadFncSerialized(tid);
#else
#error "THREADFNC_MODE must be 0 (parallel) or 1 (serialized)."
#endif
}


int main() {
	// Set global data in queue.
	vecValue initialVal;
	initialVal.tid = 0;
	initialVal.value = 42;
	values.push_back(initialVal);
	
	// Start the threads, wait for them to finish.
	thread tr1(threadFnc, 1);
	thread tr2(threadFnc, 2);
	thread tr3(threadFnc, 3);
	thread tr4(threadFnc, 4);
	
	tr1.join();
	tr2.join();
	tr3.join();
	tr4.join();
	
	// Read the calculated values.
	cout << "\n~~~~~~~~~~~~~~~Input: " << values[0].value << "\n" <<
        ", Result 1: tid " << values[1].tid << ", value: " << values[1].value << "\n" <<
        ", Result 2: tid " << values[2].tid << ", value: " << values[2].value << "\n" <<
        ", Result 3: tid " << values[3].tid << ", value: " << values[3].value << "\n" <<
        ", Result 4: tid " << values[4].tid << ", value: " << values[4].value << "\n";
	
	
	return 0;
}