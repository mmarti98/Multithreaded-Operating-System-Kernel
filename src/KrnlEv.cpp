#include "Schedule.H"
#include "Global.h"
#include "PCB.h"
#include "Event.h"
#include "KrnlEv.h"
#include "Entry.h"

KernelEv::KernelEv(IVTNo entry_) {
	owner = Global::running;
	value = 1;
	isBlocked = 0;
	num = entry_;
	IVTEntry::entry[entry_]->myEv = this;
}

void KernelEv::wait() {
	if (Global::running != owner) return;
	if (Global::running == owner) {
		if (value == 1) { value = 0; return;}
		else if (value == 0 && isBlocked == 0)
		{
			isBlocked = 1;
			Global::running->state = 4;
			dispatch();
		}
	}
}

void KernelEv::signal() {
	if (isBlocked == 0) {value = 1; return;}
	else if (isBlocked == 1)
	{
		isBlocked = 0;
		owner->state = 2;
		Scheduler::put(owner);
	}
}
KernelEv::~KernelEv() {
	owner = 0;
	IVTEntry::entry[num]->myEv = 0;
}
