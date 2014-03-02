#include <iostream>
#include <atomic>
#include <pthread.h>
#include <TASLock.h>
#include <TTASLock.h>
#include <BackoffLock.h>
#include <ctime>
#include <fstream>
#include <sys/time.h>

#define NUM_INC 10000

using namespace std;

int myGlobal = 0;


struct ThreadData
{
	Lock* myLock;
	int thread_id;
};

void *IncrementGlobal(void* threadData)
{
	struct ThreadData* data = static_cast<ThreadData*>(threadData);
	Lock& threadLock = *(data->myLock);

	for(int i = 0 ; i < NUM_INC; ++i)
	{
		threadLock.lock();
		++myGlobal;
		threadLock.unlock();
	}

	pthread_exit(NULL);
}

void CreateAndJoinThreads(int numThreads, pthread_t threads[], void* (*threadFuncPtr)(void*), Lock* lock)
{
	struct ThreadData data;
	data.myLock = lock;
	for (int i = 0 ; i < numThreads ; ++i)
	{
		int ret = pthread_create(&threads[i], NULL, threadFuncPtr, (void*)(&data));
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

double RunAndMeasureTimeHelper(int numThreads, void *(*threadFuncPtr)(void*), Lock* lock)
{
	pthread_t threads[numThreads];
	
	cout << "Creating " << numThreads << " threads" <<  endl;

	myGlobal = 0;

	struct timeval start, end;
	gettimeofday(&start, NULL);

	CreateAndJoinThreads(numThreads, threads, threadFuncPtr, lock);
	
	gettimeofday(&end, NULL);

	cout << "After each thread incrementing the global " << NUM_INC << " times, global value is " << myGlobal << endl;

	return ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0);

}

double TestTASLock(int numThreads)
{
	TASLock myTASLock;
	return RunAndMeasureTimeHelper(numThreads, IncrementGlobal, &myTASLock);
}

double TestTTASLock(int numThreads)
{
	TTASLock myTTASLock;
	return RunAndMeasureTimeHelper(numThreads, IncrementGlobal, &myTTASLock);
}

double TestBackoffLock(int numThreads)
{
	BackoffLock myBackoffLock;
	return RunAndMeasureTimeHelper(numThreads, IncrementGlobal, &myBackoffLock);
}


int main()
{
	const int maxThreads = 75;
	double TASLockTime[maxThreads], TTASLockTime[maxThreads], BackoffLockTime[maxThreads];

	for(int i = 1 ; i <= maxThreads ; ++i){
		TASLockTime[i-1] = TestTTASLock(i);
	}

	for(int i = 1 ; i <= maxThreads ; ++i){
		TTASLockTime[i-1] = TestTTASLock(i);
	}

	for(int i = 1 ; i <= maxThreads ; ++i){
		BackoffLockTime[i-1] = TestBackoffLock(i);
	}

	ofstream outFile;
	outFile.open("locktimes.csv");	
	for(int i = 0 ; i < maxThreads ; ++i){
		outFile << (i+1) << "," << TASLockTime[i] << "," << TTASLockTime[i] << "," << BackoffLockTime[i] << endl;
	}
	outFile.close();

	return 0;
}
