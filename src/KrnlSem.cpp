#include "Schedule.h"
#include "Global.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBLst.h"
#include "Semaphor.h"
#include "KrnlSem.h"
#include "SemLst.h"
#include "SleepLst.h"

void KernelSem::update() {
	if (sleeping->head)
	{
		if (sleeping->head->time > 0)
			sleeping->head->time--;
		while (sleeping->head && sleeping->head->time == 0)
		{
			SleepList::Node* wakeUp = sleeping->head;
			sleeping->head = sleeping->head->next;
			wakeUp->next = 0;
			wakeUp->pcb->passedThanksTo = 0;
			wakeUp->pcb->state = 2;
			if (wakeUp->pcb != Global::idle) Scheduler::put(wakeUp->pcb);
			blocked->PCBdelete(wakeUp->pcb);
			delete wakeUp;
			sleeping->nodes--;
			value++;
		}
	}
}
int KernelSem::val() const {return value;}

KernelSem::KernelSem( Semaphore* sem_, int inic_) {
	((inic_ < 0) ? value = 0 : value = inic_);
	mySem = sem_;
	blocked = new PCBList();
	sleeping = new SleepList();
}
int KernelSem::wait(Time maxTimeToWait) {

	if (--value < 0) {
		Global::running->state = 4;
		if (blocked != 0) blocked->PCBin(Global::running);
		if (maxTimeToWait > 0) {
			if (sleeping != 0) sleeping->putToSleep(Global::running, maxTimeToWait);
		}
		dispatch();
		if (Global::running->passedThanksTo == 0) return 0;
		if (Global::running->passedThanksTo == 1) return 1;
	}
	return 1; 
}

int KernelSem::signal(int n) {
	if (n < 0) {
		return n;
	}
	if (n == 0) {
		if (value++ < 0)
		{
			PCB* ret = blocked->PCBout();
			if (ret)
			{
				sleeping->wakeMeUp(ret);
				ret->state = 2;
				ret->passedThanksTo = 1;
				if (ret != Global::idle) Scheduler::put(ret);
			}
		}
		return 0;
	}
	else if (n > 0) {
		unsigned m = n; //
		unsigned max = blocked->getNodes();
		if (n > max) m = max;
		if (value < 0)
		{

			for (int i = 0; i < m; i++)
			{
				PCB* ret = blocked->PCBout();
				if (ret)
				{
					sleeping->wakeMeUp(ret);
					ret->state = 2;
					ret->passedThanksTo = 1;
					if (ret != Global::idle) Scheduler::put(ret);
				}
			}

		}
		value += n;
		return n;
	}
	return 0;
}

KernelSem::~KernelSem() {
	while (value < 0) { signal(0); }
	delete sleeping;
	delete blocked;
	mySem = 0;
}
