#ifndef _kernelEv_h_
#define _kernelEv_h_

class Event;
class PCB;
class IVTEntry;
typedef unsigned char IVTNo;
class KernelEv {
public:
	PCB* owner;
	unsigned value;
	unsigned isBlocked;
	IVTNo num;
	KernelEv(IVTNo entry_);
	void wait();
	void signal();
	~KernelEv();
	friend class Event;
};
#endif
