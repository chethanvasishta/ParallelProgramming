#ifndef AQLOCK_H
#define AQLOCK_H

#include <atomic>
#include <Lock.h>

class AQLock : public Lock
{
public:
	AQLock(int numThreads);
	~AQLock();

	void lock();
	void unlock();
private:
	std::atomic<int> tail;
	bool* flag;
	static __thread int mySlotIndex;
	int numThreads;
	AQLock();

};

#endif //AQLOCK_H
