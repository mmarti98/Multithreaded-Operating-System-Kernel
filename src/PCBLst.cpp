#include "Global.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBLst.h"
PCBList::PCBList() {
	head = 0; tail = 0; nodes = 0;
}
void PCBList::PCBin(PCB* pcb_) {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Node* newNode = new Node();
	newNode->pcb = pcb_;
	newNode->next = 0;
	((head != 0) ? tail->next = newNode : head = newNode);
	tail = newNode;
	nodes++;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
int PCBList::getNodes() { return nodes;}
Thread* PCBList::ThreadIdOut(int id_)
{
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif

	if (id_ < 0 || head == 0)
	{
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionEnds
#endif
		return 0;
	}
	Node* t = head;
	while ( (t != 0) && (t->pcb->id != id_) ) { t = t->next; }

	if (t == 0)
	{
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionEnds
#endif
		return 0;
	}
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
	return t->pcb->myThread;
}

PCB* PCBList::PCBout() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif

	if (head == 0)
	{
#ifndef BCC_BLOCK_IGNORE
		CriticalSectionEnds
#endif
		return 0;
	}
	Node* t = head;
	head = head->next;
	if (head == 0) tail = 0;
	nodes--;
	t->next = 0;
	PCB* headPCB = t->pcb;
	delete t;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
	return headPCB;
}

void PCBList::PCBdelete(PCB* pcb_) {
	Node* p = 0;
	Node* t = head;
	while ( (t != 0) && ( t->pcb != pcb_)) { p = t; t = t->next; }
	if (t == 0) {return;}
	if (t != 0) {
		nodes--;
		if (p != 0)
		{
			p->next = t->next;
			t->next = 0;
			delete t;
			if (p->next == 0) tail = p;
		}
		else {
			head = head->next;
			if (head == 0) tail = 0;
			t->next = 0;
			delete t;
		}
	}
}
PCBList::~PCBList() {
	while (head != 0) {
		Node*	t = head;
		head = head->next;
		delete t;
	}
	head = 0; tail = 0; nodes = 0;
}
