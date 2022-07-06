/*
 * kernel.h
 *
 *  Created on: Jun 30, 2020
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "timer.h"
#include "const.h"
#include "idleth.h"
#include "pcb.h"
#include <iostream.h>

extern int syncPrintf (const char* format, ... );

class Kernel{
public:
	static void inic();
	static void restore();

	static IdleThread * idleThread;
	//	static Thread *const mainThread;
	static PCB* getIdlePCB();
	//	static PCB* getMainPCB();
	static PCB* mainPCB;
};

/*class MainThread : public Thread {
public:
	MainThread() : Thread(4096, 1) {}
protected:
	virtual void run();

};*/


#endif /* KERNEL_H_ */
