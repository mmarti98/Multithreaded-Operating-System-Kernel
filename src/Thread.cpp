#include <iostream.h>
#include "Schedule.H"
#include "Global.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBLst.h"

Thread::Thread(StackSize stackSize_, Time timeSlice_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	myPCB = new PCB(this, stackSize_, timeSlice_);
	Global::allPCBs->PCBin(myPCB);
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
ID Thread::getId() { return ((myPCB != 0) ? myPCB->id : -501); }
ID Thread::getRunningId() { return ((Global::running != 0) ? Global::running->id : -501); }
Thread* Thread::getThreadById(ID id) {
	if (id > Global::allPCBs->getNodes()) return 0;
	return Global::allPCBs->ThreadIdOut(id);
}
void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	if ( (myPCB == 0) || (myPCB->calledStart == 1)) {
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionEnds
#endif
		return;
	}
	if (myPCB != 0 && myPCB->state == 1) {
		myPCB->calledStart = 1;
		myPCB->state = 2;
		if (myPCB != Global::idle) {Scheduler::put(myPCB); }
	}
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
void Thread::waitToComplete() {
	if (myPCB == 0) return;
	if ((myPCB != 0) && (myPCB == Global::running || myPCB == Global::idle || myPCB == Global::maint)) {return;}
	if ((myPCB != 0) && (Global::running == Global::idle) ) return;
	if ((myPCB != 0) && (myPCB->state == 1 || myPCB->state == 5)) {return;}
	if (((myPCB != 0) && (myPCB->state == 2 || myPCB->state == 4))) {
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionStarts
#endif
		Global::running->state = 4;
		myPCB->waitingMe->PCBin(Global::running);
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionEnds
#endif
		dispatch();
	}
}
void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Global::changeRunning = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	waitToComplete();
	Global::allPCBs->PCBdelete(myPCB);
	if (myPCB != 0) delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
