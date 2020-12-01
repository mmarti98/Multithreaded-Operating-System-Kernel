#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class Global;
class KernelSem;
class SemList;
class Semaphore {
public:
	Semaphore (int init_ = 1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait_);
	virtual int signal(int n_ = 0);
	int val () const;
	friend void interrupt timer();
	friend class Global;
	friend class KernelSem;
	friend class SemList;
private:
	KernelSem* myImpl;
};
#endif
