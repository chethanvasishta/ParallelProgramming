#include <iostream>
#include <atomic>
#include <pthread.h>
#include <TASLock.h>

#define NUM_THREADS 5
#define NUM_INC 1000

using namespace std;

int myGlobal = 0;

TASLock myLock;

void *IncrementGlobalTASLock(void* threadID)
{
	myLock.lock();

	//cout << "Lock acquired; Thread ID : " << threadID << " Global Value : " << myGlobal << endl;
	
	for(int i = 0 ; i < NUM_INC; ++i)
		++myGlobal;

	//cout << "Releasing lock; Thread ID : " << threadID << " Global Value : " << myGlobal << endl;

	myLock.unlock();

	pthread_exit(NULL);
}

void TestTASLock()
{
	pthread_t threads[NUM_THREADS];
	
	cout << "Creating " << NUM_THREADS << " threads" <<  endl;

	for (int i = 0 ; i < NUM_THREADS ; ++i)
	{
		int ret = pthread_create(&threads[i], NULL, IncrementGlobalTASLock, (void*)i);
		if(ret)
		{
			cout << "Pthread create error : " << ret << endl;
			return;
		}
	}

	for (int i = 0 ; i < NUM_THREADS ; ++i)
	{
		void* status;
		int ret = pthread_join(threads[i], &status);
		if(ret)
		{
			cout << "Pthread join error : " << ret << endl;
			return;
		}
	}

	cout << "After each thread incrementing the global " << NUM_INC << " times, global value is " << myGlobal << endl;

	pthread_exit(NULL);
}

int main()
{
	TestTASLock();
		
	return 0;
}
