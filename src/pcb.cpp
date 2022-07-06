/*
 * pcb.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include <dos.h>
#include "list.h"

#include <iostream.h>
extern int syncPrintf (const char* format, ... );

static int identify=0;

volatile PCB* running=0;
//volatile List<PCB*> allPCB=0;

 List<PCB *> allPCB;// zasto ovde ne moze sa nullptr


PCB::PCB(StackSize stackS,Time timeS,Thread* Th){//
	lock
	PCB::savedLock=0;
	unblockedBcTime=0;
	timeToWait=0;

	if(stackS>65536) stackSize=65536;
	stackSize=(stackS/sizeof(unsigned));
	//stackSize=stackS;
	 timeSlice=timeS;
	 myThread=Th;
	 id=++identify;
//if(==Kernel::idle)id=0;
	 status =NEW;

//	 syncPrintf( "konstr pcb: %d\n ",id);


	// if(stackSize>0/* && <65535bajtova/ size*/){

	stack= new unsigned[stackSize];

	stack[stackSize-1] = 0x200;//setovan I fleg u
	//stack[stackSize-12] = 0;                             // pocetnom PSW-u za nit
	#ifndef BCC_BLOCK_IGNORE
		//stack[stackSize-2] = FP_SEG(&(PCB::wrapper));
		//stack[stackSize-3] = FP_OFF(&(PCB::wrapper));

	stack[stackSize-2] = FP_SEG(wrapper);
	stack[stackSize-3] = FP_OFF(wrapper);

		sp = FP_OFF(stack+stackSize-12);
		bp = FP_OFF(stack+stackSize-12);
		ss = FP_SEG(stack+stackSize-12);
	#endif
	// }
	// else stack=nullptr;// ili greska? ili bolje odrazumevana


	allPCB.addEnd(this);
	unlock
 }

PCB::PCB(){
	lock
savedLock=0;
	unblockedBcTime=0;
	timeToWait=0;
	stack=nullptr;
	ss=0;
	sp=0;
	bp=0;
	timeSlice=0;
	//timeSlice=defaultTimeSlice;
	id=++identify;
	//syncPrintf("PCB konstr za main PCB: %d\n",id);
	myThread=nullptr;
	//status= running
	stackSize=0;
//	status=NEW;//ako nije new mora da se promeni u startu
	status=READY;//jer svakako ne startujemo
	//allPCB.addEnd((PCB*)this);
	allPCB.addEnd(this);
	unlock
}
PCB::~PCB(){
	lock
	//unblockWaiting();
	//	blockedList.deleteList();
	//syncPrintf("PCB- destruktor\n");
	for(allPCB.onFirst();allPCB.currentExists(); allPCB.onNext()){
		PCB* tmp= *allPCB.getCurrent();//if curr exists
		if(tmp->id==this->id){
			//allPCB.getCurrent()=nullptr;/////? kazem da je elem.data tj pcb =null
			allPCB.deleteCurrent();
			break;
		}
	}

	if(stack!=nullptr){
		lock
		delete []stack;
		unlock
		stack=nullptr;

	}
	unlock
}

void PCB:: wrapper(){

	//syncPrintf("wrapper pre runn: %d \n", running->id);
//while(running->status==NEW){}
	running->myThread->run();// u tekstu stoji PCB::
	lock


	running->status=FINISHED;
	((PCB*)running)->unblockWaiting();

	//syncPrintf( "wrapper posle unblock: %d \n",running->id);
	unlock
	dispatch();

	return;
}

/*void PCB::unblockWaiting(){
	lock
	///List::Elem * tmp;//=PCB::blockedList.head;// zasto nece ovalko
	//for(tmp=blockedList.head; tmp!=nullptr;tmp=tmp->next){}
	if(blockedList.head==nullptr){unlock return;}
	for(blockedList.onFirst();blockedList.currentExists();blockedList.onNext()){
		PCB * tmp= blockedList.getCurrent();
		tmp->status= READY;
		Scheduler::put(tmp);
		//blockedList.deleteCurrent();//brise se original? plus zato sto je for petlja ako brisem poremetice se
		blockedList.getCurrent()=nullptr;//odvezem od pcba
	}
blockedList.deleteList();////?????????????? sad kad je odvezano mogu da brisem
unlock
}*/
void PCB::unblockWaiting(){
	lock

	if(blockedList.head==nullptr){unlock return;}
	for(blockedList.onFirst();blockedList.currentExists();){//blockedList.onNext()){///////////////////////////DELETE CURR POMERA
		PCB * tmp= *blockedList.getCurrent();
		tmp->status= READY;
		Scheduler::put(tmp);
		//blockedList.deleteCurrent();//brise se original? plus zato sto je for petlja ako brisem poremetice se
		//////////
		tmp=nullptr;//odvezem
		blockedList.deleteCurrent();//brise se original? plus zato sto je for petlja ako brisem poremetice se

	}
unlock
}
