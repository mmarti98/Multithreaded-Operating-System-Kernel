#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;
class Global;
class PCB;
class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id_);
protected:
	friend class PCB;
	friend class Global;
	Thread (StackSize stackSize_ = defaultStackSize, Time timeSlice_ = defaultTimeSlice);
	virtual void run() {}
private:
	PCB* myPCB;
};

void dispatch ();

#endif
