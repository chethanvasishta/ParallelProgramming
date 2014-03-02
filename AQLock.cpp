#include <AQLock.h>
#include <unistd.h> //sleep
#include <algorithm>

__thread int AQLock::mySlotIndex;

AQLock::AQLock(int n)
{
	numThreads = n;
	tail = 0;
	flag = new bool[numThreads]; //Initialize to false?
	flag[0] = true;
}

AQLock::~AQLock()
{
	delete[] flag;
	flag = NULL;
}

void AQLock::lock()
{
	int slot = tail++ % numThreads;
	mySlotIndex = slot;
	while(! flag[slot]);
	
}

void AQLock::unlock()
{
	int slot = mySlotIndex;
	flag[slot] = false;
	flag[(slot+1) % numThreads] = true;
}

