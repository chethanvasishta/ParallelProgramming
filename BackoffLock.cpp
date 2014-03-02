#include <BackoffLock.h>
#include <unistd.h> //sleep
#include <algorithm>

BackoffLock::BackoffLock() : backoff(100, 1000)
{
	isLocked = false;
	numBackoffs = 9;
}

BackoffLock::~BackoffLock()
{
}

void BackoffLock::lock()
{
	backoff.reset();
	while(true)
	{
		while(isLocked);
		if(!isLocked.exchange(true))
			return;
		++numBackoffs;
		backoff.backoff();		
	}
}

void BackoffLock::unlock()
{
	isLocked = false;
}

Backoff::Backoff(int min, int max)
{
	minDelay = min;
	maxDelay = max;
	limit = min;
}

void Backoff::backoff()
{
	limit = std::max(2 * limit, maxDelay);
	usleep(limit);
}
