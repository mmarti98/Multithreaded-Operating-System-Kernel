#include <dos.h>
#include "Schedule.H"
#include "Global.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBLst.h"
#include "Idle.h"
#include "KrnlSem.h"
int PCB::lastId = 0;
PCB::PCB(Thread* th_, StackSize sizeStack_, Time timeSlice_) {
	myThread = th_;
	calledStart = 0;
	passedThanksTo = 0;
	id = ++PCB::lastId;
	size = sizeStack_;
	time = timeSlice_;
	((time == 0) ? endless = 1 : endless = 0);
	state = 1;
	if (size > 65536) size = 65536;
	waitingMe = new PCBList();
	size = size / sizeof(unsigned);
	stack = new unsigned[size];
#ifndef BCC_BLOCK_IGNORE
	stack[size - 1] = 0x200;
	stack[size - 2] = FP_SEG(&(PCB::wrapper));
	stack[size - 3] = FP_OFF(&(PCB::wrapper));
	sp = FP_OFF(stack + size - 12);
	ss = FP_SEG(stack + size - 12);
#endif
	bp = sp;
}
void PCB::wrapper() {
	Global::running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Global::running->state = 5;
	PCB* iWait = Global::running->waitingMe->PCBout();
	int x = Global::running->waitingMe->getNodes();
	while (iWait != 0)
	{
		iWait->state = 2;
		if (Global::running != Global::idle) { 	Scheduler::put(iWait); }
		iWait = Global::running->waitingMe->PCBout();
	}

#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
	dispatch();
}

PCB::~PCB() {
	myThread = 0;
	if (stack != 0) delete []stack;
	if (waitingMe != 0) delete waitingMe;
}
