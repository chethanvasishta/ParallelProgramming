#include <BackoffLock.h>
#include <unistd.h> //sleep
#include <algorithm>

BackoffLock::BackoffLock()
{
	isLocked = false;
}

BackoffLock::~BackoffLock()
{
}

void BackoffLock::lock()
{
	Backoff backoff(10, 100);
	while(true)
	{
		while(isLocked);
		if(!isLocked.exchange(true))
			return;
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
