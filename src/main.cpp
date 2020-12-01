#include <stdio.h>
#include <iostream.h>
#include "Global.h"
#include "PCBLst.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Global::inic();
	int result = userMain(argc, argv);
	Global::restore();
	if (Global::allPCBs != 0) {
		int x = Global::allPCBs->getNodes();
		printf("\n ukupno u globalnoj listi %d", x);
	}
	return result;
}
