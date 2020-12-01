#include <iostream.h>
#include <dos.h>
#include "Global.h"
#include "Event.h"
#include "KrnlEv.h"
#include "Entry.h"

IVTEntry* IVTEntry::entry[256] = {0};
IVTEntry::IVTEntry(IVTNo entry_, PrekidnaR nRoutine_) {
	num = entry_;
	myEv = 0;
	entry[entry_] = this;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts

	oRoutine = getvect(num);
	setvect(num, nRoutine_);

	CriticalSectionEnds
#endif
}
void IVTEntry::calloldRoutine()
{
	if (oRoutine) oRoutine();

}
IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
	setvect(num, oRoutine);
#endif
	calloldRoutine();
	oRoutine = 0;
	entry[num] = 0;
	myEv = 0;
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
void IVTEntry::signal() {
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionStarts
#endif
	if (myEv != 0) myEv->signal();
#ifndef BCC_BLOCK_IGNORE
	CriticalSectionEnds
#endif
}
