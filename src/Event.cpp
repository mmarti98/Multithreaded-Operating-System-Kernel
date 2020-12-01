#include "Schedule.h"
#include "Global.h"
#include "Event.h"
#include "KrnlEv.h"
#include "Entry.h"
Event::Event (IVTNo ivtNo_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	myImpl = new KernelEv(ivtNo_);
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
void Event::wait() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
void Event::signal() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}

Event::~Event() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	if (myImpl != 0) delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
