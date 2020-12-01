#ifndef _semList_h_
#define _semList_h_

class Global;
class KernelSem;
class Semaphore;
class Thread;
class SemList {
public:
	struct Node {
		KernelSem* sem;
		Node* next;
	};
	Node* head;
	Node* tail;
	int nodes;
	SemList();
	int getNodes() {return nodes;}
	void semIn(KernelSem* sem_);
	void semOut(KernelSem* sem_);
	~SemList();
	friend void interrupt timer();
	friend class Global;
	friend class KernelSem;
};

#endif
