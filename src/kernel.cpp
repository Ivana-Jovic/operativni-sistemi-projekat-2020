/*
 * kernel.cpp
 *
 *  Created on: Jun 30, 2020
 *      Author: OS1
 */
#include "kernel.h"

extern int syncPrintf (const char* format, ... );
//Thread *const Kernel::mainThread;// = new Thread();

IdleThread * Kernel::idleThread;// = new IdleThread();
PCB* Kernel::mainPCB;


void Kernel::inic(){
	lock
	mainPCB= new PCB();
	unlock

	mainPCB->status= READY;
 	running= mainPCB;

	lock
 	Kernel::idleThread = new IdleThread();
	//Kernel::idleThread->start();
	Kernel::idleThread->myPCB->status=READY;
	unlock

//	syncPrintf("IDLE NIT napravljen u kernel inic : %d\n",Kernel::getIdlePCB()->id);
	Timer::inic();
}

void Kernel::restore(){
	Timer::restore();
lock
	delete mainPCB;//?
//	delete Kernel::idleThread;//ne sme jer ce se pozvati wait to compl u destr
unlock
}

PCB* Kernel::getIdlePCB() {
	return idleThread->myPCB;
}
			/*PCB* Kernel::getMainPCB() {
				return mainThread->myPCB;
			}
			*/
