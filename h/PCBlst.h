#ifndef _pcbList_h_
#define _pcbList_h_

class Thread;
class PCB;
class PCBList {
public:
	struct Node {
		PCB* pcb;
		Node* next;
	};
	Node* head;
	Node* tail;
	int nodes;
	PCBList();
	int getNodes();
	Thread* ThreadIdOut(int id_);
	void PCBin(PCB* pcb_);
	PCB* PCBout();
	void PCBdelete(PCB* pcb_);
	~PCBList();
	friend class Thread;
	friend class PCB;
};
#endif
