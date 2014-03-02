#ifndef BACKOFFLOCK_H
#define BACKOFFLOCK_H

#include <atomic>
#include <Lock.h>

class Backoff
{
public:
	Backoff(int min, int max);	
	void backoff();
	void reset() { limit = minDelay; }

private:
	int minDelay, maxDelay, limit;
	Backoff();
};

class BackoffLock : public Lock
{
public:
	BackoffLock();
	~BackoffLock();

	void lock();
	void unlock();
	long NumBackoffs() { return numBackoffs; }
private:
	std::atomic<bool> isLocked;	
	Backoff backoff;
	long numBackoffs;
};

#endif //BACKOFFLOCK_H
