#ifndef TTASLOCK_H
#define TTASLOCK_H

#include <atomic>

class TTASLock
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
