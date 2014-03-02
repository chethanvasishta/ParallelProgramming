#include <iostream>
#include <atomic>
#include <pthread.h>
#include <TASLock.h>
#include <TTASLock.h>
#include <ctime>
#include <fstream>
#include <sys/time.h>

#define NUM_INC 10000

using namespace std;

int myGlobal = 0;

TASLock myTASLock;

void *IncrementGlobalTASLock(void* threadID)
{

	for(int i = 0 ; i < NUM_INC; ++i)
	{

		myTASLock.lock();
		++myGlobal;
		myTASLock.unlock();
	}

	pthread_exit(NULL);
}

TTASLock myTTASLock;

void *IncrementGlobalTTASLock(void* threadID)
{

	for(int i = 0 ; i < NUM_INC; ++i)
	{
		myTTASLock.lock();
		++myGlobal;
		myTTASLock.unlock();
	}

	pthread_exit(NULL);
}

void CreateAndJoinThreads(int numThreads, pthread_t threads[], void* (*threadFuncPtr)(void*))
{
	for (int i = 0 ; i < numThreads ; ++i)
	{
		int ret = pthread_create(&threads[i], NULL, threadFuncPtr, (void*)i);
		if(ret)
		{
			cout << "Pthread create error : " << ret << endl;
			return;
		}
	}

	for (int i = 0 ; i < numThreads ; ++i)
	{
		void* status;
		int ret = pthread_join(threads[i], &status);
		if(ret)
		{
			cout << "Pthread join error : " << ret << endl;
			return;
		}
	}
}

double TestTASLock(int numThreads)
{
	pthread_t threads[numThreads];
	
	cout << "Creating " << numThreads << " threads" <<  endl;

	myGlobal = 0;

	struct timeval start, end;
	gettimeofday(&start, NULL);

	CreateAndJoinThreads(numThreads, threads, IncrementGlobalTASLock);
	
	gettimeofday(&end, NULL);

	cout << "After each thread incrementing the global " << NUM_INC << " times, global value is " << myGlobal << endl;

	return ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0);
}

double TestTTASLock(int numThreads)
{
	pthread_t threads[numThreads];
	
	cout << "Creating " << numThreads << " threads" <<  endl;

	myGlobal = 0;

	struct timeval start, end;
	gettimeofday(&start, NULL);

	CreateAndJoinThreads(numThreads, threads, IncrementGlobalTTASLock);

	gettimeofday(&end, NULL);

	cout << "After each thread incrementing the global " << NUM_INC << " times, global value is " << myGlobal << endl;

	return ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0);
}

int main()
{
	const int maxThreads = 100;
	double TASLockTime[maxThreads], TTASLockTime[maxThreads];

	for(int i = 1 ; i <= maxThreads ; ++i){
		TASLockTime[i-1] = TestTTASLock(i);
	}

	for(int i = 1 ; i <= maxThreads ; ++i){
		TTASLockTime[i-1] = TestTTASLock(i);
	}

	ofstream outFile;
	outFile.open("locktimes.csv");	
	for(int i = 0 ; i < maxThreads ; ++i){
		outFile << (i+1) << "," << TASLockTime[i] << "," << TTASLockTime[i] << endl;
	}
	outFile.close();

	return 0;
}
