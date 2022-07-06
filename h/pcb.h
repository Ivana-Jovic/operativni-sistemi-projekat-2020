/*
 * pcb.h
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_


#include "thread.h"
#include "const.h"
#include "SCHEDULE.H"
#include "list.h"

extern volatile PCB* running;// volaile jer se menja u pekidnoj rutini

extern  List<PCB *> allPCB;

enum Status {NEW, READY, BLOCKED, FINISHED};

class PCB{
public:	unsigned ss;
	unsigned sp;
	unsigned bp;


//	static volatile PCB* running=0;
	ID id;
	Status status;

	unsigned * stack;
	StackSize stackSize;
	Time timeSlice;
	Thread * myThread;

	//static List<PCB*> allPCB;

	List<PCB *> blockedList;
	int savedLock;// ovo se koristi za lock u timeru
//za semafore:
	int unblockedBcTime;
	volatile int timeToWait;//IZMENA 18.8

	PCB(StackSize stackS,Time timeS,Thread* Thread);
	PCB();
	~PCB();

	static void wrapper();//rekli da je static u tekstu


	void unblockWaiting();

};



#endif /* PCB_H_ */
