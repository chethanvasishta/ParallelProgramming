#ifndef LOCK_H
#define LOCK_H

//Abstract base class for locks
class Lock
{
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

#endif //LOCK_H
