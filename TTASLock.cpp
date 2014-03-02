#include <TTASLock.h>

TTASLock::TTASLock()
{
	isLocked = false;
}

TTASLock::~TTASLock()
{
}

void TTASLock::lock()
{
	while(true)
	{
		while(!isLocked);
		if(!isLocked.exchange(true))
			return;
	}
}

void TTASLock::unlock()
{
	isLocked = false;
}


