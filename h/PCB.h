#ifndef _pcb_h_
#define _pcb_h_

class Global;
class SleepList;
class SemList;
class KernelSem;
class KernelEv;
class PCBList;
class Thread;
typedef unsigned long StackSize;
typedef unsigned int Time;
class PCB {
public:
	Thread* myThread;
	PCBList* waitingMe;
	int endless;
	Time time;
	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	StackSize size;
	unsigned passedThanksTo;
	unsigned state;
	static int lastId;
	int id;
	unsigned calledStart;
	PCB(Thread* th_, StackSize sizeStack_ , Time timeSlice_);
	~PCB();
	static void wrapper();
	friend void interrupt timer();
	friend class Global;
	friend class Thread;
	friend class KernelEv;
	friend class KernelSem;
	friend class SleepList;
	friend class PCBList;
	friend class SemList;
};

#endif
