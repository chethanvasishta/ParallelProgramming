#ifndef TASLOCK_H
#define TASLOCK_H

#include <atomic>
#include <Lock.h>

class TASLock : public Lock
{
public:
	TASLock();
	~TASLock();

	void lock();
	void unlock();
private:
	std::atomic<bool> isLocked;
};

#endif //TASLOCK_H
