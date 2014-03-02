#ifndef BACKOFFLOCK_H
#define BACKOFFLOCK_H

#include <atomic>
#include <Lock.h>

class BackoffLock : public Lock
{
public:
	BackoffLock();
	~BackoffLock();

	void lock();
	void unlock();
private:
	std::atomic<bool> isLocked;
};

class Backoff
{
public:
	Backoff(int min, int max);	
	void backoff();

private:
	int minDelay, maxDelay, limit;
	Backoff();
};

#endif //BACKOFFLOCK_H
