#include <TASLock.h>

TASLock::TASLock()
{
	isLocked = false;
}

TASLock::~TASLock()
{
}

void TASLock::lock()
{
	while(isLocked.exchange(true));
}

void TASLock::unlock()
{
	isLocked = false;
}


