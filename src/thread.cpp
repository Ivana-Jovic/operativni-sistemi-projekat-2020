/*
 * thread.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#include "thread.h"
#include"pcb.h"
#include "const.h"
#include "SCHEDULE.H"
#include "timer.h"
#include "kernel.h"
#include <iostream.h>



extern int syncPrintf (const char* format, ... );

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock

	myPCB= new PCB(stackSize, timeSlice, this);

	//syncPrintf("  u konstr klase thread\n");
	unlock
}

void Thread::start(){
	lock
	//cout<< "u startu "<< myPCB->id<<endl;
	if(myPCB->status==NEW){// getstatus
		myPCB->status=READY;
		//if(this->myPCB!= Kernel::getIdlePCB()){

		//syncPrintf("pre scheduler put u thread.start\n");
			Scheduler::put((PCB*)(this->myPCB));

			//syncPrintf("posle scheduler put u thread.start\n");
		//}
	}
	//else greska
	//syncPrintf("kraj starta thread.start\n");


	unlock
}


void Thread::waitToComplete(){
	lock
	//syncPrintf("Thread- waitToComplete pozvan\n");
	if((running == this->myPCB) || this->myPCB->status == FINISHED ||
		this->myPCB->status == NEW /*||this->myPCB->status == BLOCKED*/
		|| this->myPCB==Kernel::getIdlePCB() || this->myPCB==Kernel::mainPCB) {
			unlock
			return;
	}


//	lock
		running->status = BLOCKED;


		myPCB->blockedList.addEnd((PCB*)running);
	unlock
		dispatch();
//	syncPrintf("Thread- waitToComplete gotov\n");

}

//virtual Thread::~Thread(){
Thread::~Thread(){
	//syncPrintf("Thread- destr pozvan\n");
	waitToComplete();
	lock
	if (myPCB!=nullptr) delete myPCB;// zove destruktor za pcb ili  eksplicitno
	myPCB=nullptr;
	unlock
}

ID Thread::getId(){
	lock
	if(myPCB!=nullptr){unlock return myPCB->id;}
	else {unlock return -10;}
}

ID Thread::getRunningId(){return running->id;}


Thread * Thread::getThreadById(ID id){
	lock
	Thread* t= nullptr;
	for(allPCB.onFirst();allPCB.currentExists();allPCB.onNext()){
		PCB* tmp=*allPCB.getCurrent();
		if(tmp->id==id){
			t=(tmp->myThread);
			break;
		}
	}
	unlock

	return t;
}


void dispatch(){
	//lock
	//cout<< "u dispatchu pre timer "<< running->id <<endl;

#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	//syncPrintf("u dispatchu pre timer\n");
	Timer::context_switch_on_demand = true;
	timer();
	//syncPrintf("u dispatchu posle timer\n");
#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif

	//	cout<< "u dispatchu posle timer\n";//<< running->myThread->getId() <<endl;
	//unlock

}

