#ifndef ATOMIC_H
#define ATOMIC_H

#include <atomic>
#include <Lock.h>

class Atomic : public Lock 
{
public:
	Atomic();
	~Atomic();

	void Increment() { m_atomicInt++; }
	void Decrement() { m_atomicInt--; }

	void lock() {} //Does nothing
	void unlock() {} //Does nothing
private:
	std::atomic<int> m_atomicInt;
};

#endif //ATOMIC_H
