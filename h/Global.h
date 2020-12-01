#ifndef _global_h_
#define _global_h_

#define CriticalSectionStarts asm { pushf; cli; }
#define CriticalSectionEnds asm popf;

class PCB;
class SemList;
class PCBList;
class Thread;
class KernelSem;
class Idle;
typedef unsigned int Time;
class Global {
public:
	volatile static Time cntCLK;
	volatile static int changeRunning;
	static PCB* maint;
	static PCB* running;
	static PCB* idle;
	static SemList* allSems;
	static PCBList* allPCBs;
	void static inic();
	void static restore();
	friend void interrupt timer();
	friend void dispatch();
	friend class Idle;
	friend class KernelSem;
	friend class Thread;
	friend class PCB;
};
void interrupt timer();
#endif
