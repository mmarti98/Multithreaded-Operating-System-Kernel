#include <stdio.h>
#include "Thread.h"
#include "Idle.h"
Idle::Idle(): Thread(defaultStackSize, 1) {
	x = 1;
}
Idle::~Idle() {
	waitToComplete();
}
void Idle::run() {
	while (1) {
		x++;
		x--;
		//	printf("i");
	}
}
