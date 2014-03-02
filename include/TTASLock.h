#ifndef TTASLOCK_H
#define TTASLOCK_H

#include <atomic>
#include <Lock.h>

class TTASLock : public Lock
{
public:
	TTASLock();
	~TTASLock();

	void lock();
	void unlock();
private:
	std::atomic<bool> isLocked;
};

#endif //TTASLOCK_H
