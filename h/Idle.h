#ifndef _idle_h_
#define _idle_h_

#include "Thread.h"

class Thread;

class Idle : public Thread {
public:
	unsigned x;
	Idle();
	void run();
	~Idle();

};
#endif
