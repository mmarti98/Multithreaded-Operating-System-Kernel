#include <dos.h>
#include "Schedule.H"
#include "Global.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBLst.h"
#include "Idle.h"
#include "Semaphor.h"
#include "KrnlSem.h"
#include "SemLst.h"
#include "SleepLst.h"
#include "Event.h"
#include "KrnlEv.h"
#include "Entry.h"
#include <iostream.h>
extern void tick();
PCBList* Global::allPCBs = new PCBList();
SemList* Global::allSems = new SemList();
PCB* Global::running = 0;
PCB* Global::maint = 0;
PCB* Global::idle = 0;
volatile Time Global::cntCLK = 2;
volatile int Global::changeRunning = 0;
unsigned int tsp;
unsigned int tss;
unsigned int tbp;
unsigned oldTimerOFF, oldTimerSEG;
void Global::inic() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	Thread* main = new Thread(defaultStackSize, defaultTimeSlice);
	main->myPCB->state = 2;
	Global::running = main->myPCB;
	Global::maint = main->myPCB;
	main = new Idle();
	main->myPCB->state = 2;
	idle = main->myPCB;
	main = 0;
#ifndef BCC_BLOCK_IGNORE
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov ax, seg timer
		mov word ptr es:0022h, ax
		mov ax, offset timer
		mov word ptr es:0020h, ax

		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
	}
	CriticalSectionEnds
#endif
}
void interrupt timer() {
	if (Global::changeRunning == 0) {
#ifndef BCC_BLOCK_IGNORE
		asm {int 60h;}
#endif
		tick();
		SemList::Node *t = Global::allSems->head;
		int x = Global::allSems->getNodes();
		for (int i = 0; i < x; i++) {
			if (t != 0) {
				t->sem->update();
				t = t->next;
			}
		}
		if (Global::running->endless == 1) {return; }
		Global::cntCLK--;
	}
	if (Global::cntCLK == 0 || Global::changeRunning == 1) {
		Global::changeRunning = 0;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		Global::running->sp = tsp;
		Global::running->ss = tss;
		Global::running->bp = tbp;
		if (Global::running->state == 2)
		{
			if (Global::running != Global::idle) {Scheduler::put(Global::running); }
		}
		Global::running = Scheduler::get();
		if ( Global::running == 0) {
			Global::running = Global::idle;
		}
		Global::cntCLK = Global::running->time;
		tsp = Global::running->sp;
		tss = Global::running->ss;
		tbp = Global::running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif
	}
}

void Global::restore() {
	delete Global::allSems;
	delete Global::allPCBs;
	Global::allPCBs = 0;
	Global::allSems = 0;
	Global::idle = 0;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
	}

	CriticalSectionEnds
#endif
}
