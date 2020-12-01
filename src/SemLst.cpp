#include <iostream.h>
#include "Global.h"
#include "Semaphor.h"
#include "KrnlSem.h"
#include "SemLst.h"

SemList::SemList() {
	head = 0; tail = 0; nodes = 0;
}
void SemList::semIn(KernelSem* sem_)
{
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Node* newNode = new Node();
	newNode->sem = sem_;
	newNode->next = 0;
	((head != 0) ? tail->next = newNode : head = newNode);
	tail = newNode;
	nodes++;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}

void SemList::semOut(KernelSem* sem_) {
	Node* p = 0;
	Node* t = head;
	while ( (t != 0) && ( t->sem != sem_)) { p = t; t = t->next; }
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

SemList::~SemList() {

	while (head != 0) {
		Node* t = head;
		head = head->next;
		delete t;
	}
	head = 0; tail = 0; nodes = 0;
}
