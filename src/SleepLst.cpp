#include <iostream.h>
#include "Schedule.H"
#include "Global.h"
#include "PCB.h"
#include "SleepLst.h"

SleepList::SleepList() {
	head = 0; nodes = 0;
}
void SleepList::putToSleep(PCB* pcb_, Time time_) {
	if (pcb_ == 0 || time_ == 0)
	{
		return;
	}
	Node* newNode = new Node();
	newNode->time = time_;
	newNode->pcb = pcb_;
	newNode->next = 0;
	nodes++;
	if (head == 0) head = newNode;
	else
	{
		if (head->time < time_)
		{
			unsigned newInfo = time_;
			Node* t = head;
			Node* p = 0;

			while (t && (t->time <= newInfo))
			{
				newInfo = newInfo - t->time;
				p = t;
				t = t->next;
			}
			Node* newNode = new Node();
			newNode->time = newInfo;
			newNode->pcb = pcb_;
			newNode->next = t;
			p->next = newNode;

			if (newNode->time != 0)
			{
				if (t)
				{
					t->time = t->time - newNode->time;
				}
			}
		}
		else if (head->time > time_)
		{
			Node* firstNode = newNode;
			head->time = head->time - time_;
			firstNode->next = head;
			head = firstNode;
		}
		else if (head->time == time_)
		{
			Node* temp = head->next;
			Node* prev = head;

			while (temp && temp->time == 0)
			{
				prev = temp;
				temp = temp->next;
			}
			Node* newN = new Node();
			newN->pcb = pcb_;
			newN->time = 0;
			newN->next = temp;
			prev->next = newN;
		}

	}

}
void SleepList::wakeMeUp(PCB* pcb_) {
	Node* t = head;
	Node* p = 0;
	while (t && t->pcb != pcb_)	{p = t;	t = t->next;}
	if (!t)		{	return;	}
	if (!p)
	{
		Node* t = head;
		head = head->next;
		t->next = 0;
		if (head != 0)head->time = head->time + t->time;
		nodes--;
		delete t;
		return;
	}
	if (p)
	{
		p->next = t->next;
		if (t->next != 0) t->next->time = t->next->time + t->time;
		t->next = 0;
		nodes--;
		delete t;
		return;
	}
}

SleepList::~SleepList() {
	while (head != 0) {
		Node*	t = head;
		head = head->next;
		delete t;
	}
	head = 0; nodes = 0;

}
