#include <iostream>
#include <atomic>
#include <pthread.h>
#include <TASLock.h>
#include <TTASLock.h>
#include <BackoffLock.h>
#include <ctime>
#include <fstream>
#include <sys/time.h>
#include <AQLock.h>
#include <Atomic.h>

#define NUM_INC 10000

using namespace std;

int myGlobal = 0;


struct ThreadData
{
	Lock* myLock;
	int thread_id;
};

void *IncrementAtomic(void* threadData)
{
	struct ThreadData* data = static_cast<ThreadData*>(threadData);
	Atomic& threadLock = *(Atomic*)(data->myLock);

	for(int i = 0 ; i < NUM_INC; ++i)
	{
		threadLock.Increment();	
	}

	pthread_exit(NULL);
}

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

double TestBackoffLock(int numThreads, long *backOff)
{
	BackoffLock myBackoffLock;
	double time = RunAndMeasureTimeHelper(numThreads, IncrementGlobal, &myBackoffLock);
	*backOff = myBackoffLock.NumBackoffs();
	return time;
}

double TestAQLock(int numThreads)
{
	AQLock myLock(numThreads);
	double time = RunAndMeasureTimeHelper(numThreads, IncrementGlobal, &myLock);
	return time;
}

double TestAtomicTime(int numThreads)
{
	Atomic myLock;
	double time = RunAndMeasureTimeHelper(numThreads, IncrementAtomic, &myLock);
	return time;
}

int main()
{
	const int maxThreads = 20, numAvgs = 25;
	double TASLockTime[maxThreads], TTASLockTime[maxThreads], BackoffLockTime[maxThreads], AQLockTime[maxThreads], AtomicTime[maxThreads];
	long backOffCount[maxThreads];
	double cumulativeTime = 0;

	for(int i = 1 ; i <= maxThreads ; ++i){
		cumulativeTime = 0;
		for(int j = 0 ; j < numAvgs ; ++j)
			cumulativeTime += TestTASLock(i);
		TASLockTime[i-1] = cumulativeTime/numAvgs;
	}

	for(int i = 1 ; i <= maxThreads ; ++i){
		cumulativeTime = 0;
		for(int j = 0 ; j < numAvgs ; ++j)
			cumulativeTime += TestTTASLock(i);
		TTASLockTime[i-1] = cumulativeTime/numAvgs;
	}

	for(int i = 1 ; i <= maxThreads ; ++i){
		cumulativeTime = 0;
		for(int j = 0 ; j < numAvgs ; ++j)
			cumulativeTime += TestBackoffLock(i, &backOffCount[i]);
		BackoffLockTime[i-1] = cumulativeTime/numAvgs;
	}

	//for(int i = 1 ; i <= maxThreads ; ++i){
	//	AQLockTime[i-1] = TestAQLock(i);	
	//}
	//TestAQLock(5);

	for(int i = 1 ; i <= maxThreads ; ++i){
		cumulativeTime = 0;
		for(int j = 0 ; j < numAvgs ; ++j)
			cumulativeTime += TestAtomicTime(i);
		AtomicTime[i-1] = cumulativeTime/numAvgs;
	}

	ofstream outFile;
	outFile.open("locktimes.csv");	
	outFile << "ThreadCount,TASLock,TTASLock,BackoffLock,BackOffCount,AtomicTime" << endl;
	for(int i = 0 ; i < maxThreads ; ++i){
		outFile << (i+1) << "," << TASLockTime[i] << "," << TTASLockTime[i] << "," << BackoffLockTime[i] << "," << backOffCount[i] << "," << AtomicTime[i] <<  endl;
	}
	outFile.close();
	
	return 0;
}
