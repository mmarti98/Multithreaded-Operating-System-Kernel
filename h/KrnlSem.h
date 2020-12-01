#ifndef _kernelSem_h_
#define _kernelSem_h_

class Semaphore;
class SemList;
class SleepList;
class PCBList;
typedef unsigned int Time;
class KernelSem {
public:
	int value;
	Semaphore* mySem;
	PCBList* blocked;
	SleepList* sleeping;
	KernelSem(Semaphore* sem_, int inic_);
	void update();
	int val() const;
	int wait(Time maxTimeToWait_);
	int signal(int n_);
	virtual ~KernelSem();
	friend class Semaphore;
	friend class SemList;
	friend class PCBList;
	friend void interrupt timer();
};
#endif
