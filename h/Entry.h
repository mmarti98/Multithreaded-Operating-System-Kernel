#ifndef _entry_h_
#define _entry_h_

#define PREPAREENTRY(num,flag)\
void interrupt routine##num(...);\
IVTEntry entry##num(num,routine##num);\
void interrupt routine##num(...) {\
entry##num.signal();\
if (flag == 1) entry##num.calloldRoutine();\
dispatch();\
}\

class Event;
class KernelEv;
typedef void interrupt (*PrekidnaR)(...);
typedef unsigned char IVTNo;
class IVTEntry {
public:
	IVTNo num;
	KernelEv* myEv;
	PrekidnaR oRoutine;
	static IVTEntry* entry[];
	IVTEntry(IVTNo entry_, PrekidnaR nRoutine_);
	~IVTEntry();
	void signal();
	void calloldRoutine();
	friend class KernelEv;
	friend class Event;
};

#endif
