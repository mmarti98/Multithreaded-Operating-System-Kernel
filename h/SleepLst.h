#ifndef _sleepList_h_
#define _sleepList_h_

class Global;
class PCB;
typedef unsigned int Time;
class SleepList
{
public:
	struct Node {
		PCB* pcb;
		Time time;
		Node* next;
	};
	Node* head;
	int nodes;
	SleepList();
	int getNodes() {return nodes;}
	void putToSleep(PCB* pcb_, Time time_);
	void wakeMeUp(PCB* pcb_);
	~SleepList();
	friend void interrupt timer();
	friend class Global;
};

#endif
