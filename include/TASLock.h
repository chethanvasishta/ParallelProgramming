#ifndef TASLOCK_H
#define TASLOCK_H

#include <atomic>

class TASLock
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
