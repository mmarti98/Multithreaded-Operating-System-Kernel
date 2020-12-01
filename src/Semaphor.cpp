#include <iostream.h>
#include "Global.h"
#include "PCB.h"
#include "Semaphor.h"
#include "KrnlSem.h"
#include "SleepLst.h"
#include "SemLst.h"

Semaphore::Semaphore(int init_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	myImpl = new KernelSem(this, init_);
	Global::allSems->semIn(myImpl);
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
int Semaphore::wait(Time maxTimeToWait_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	int ret = myImpl->wait(maxTimeToWait_);
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
	return ret;
}
int Semaphore::signal(int n_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	int ret = myImpl->signal(n_);
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
	return ret;
}
int Semaphore::val() const {return myImpl->val();}

Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Global::allSems->semOut(myImpl);
	if (myImpl)	delete myImpl;

#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
