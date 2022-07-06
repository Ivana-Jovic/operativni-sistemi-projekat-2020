/*
 * kersem.h
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

#include "list.h"
#include "pcb.h"
#include "const.h"
//extern List<KernelSem *> semaphores;
void semaphoreTimeDecr();

class KernelSem{
public:
	volatile int semVal;//volatile!

	List<PCB *> waitingThreadsOnSem;
	static List<KernelSem *> semaphores;


	KernelSem(int init);
	~ KernelSem();
	int waitKernel(Time maxTimeToWait);// da li su i ovde virtual kao tamo
	int signalKernel(int n);//=0?
	int valKernel () const;

};


#endif /* KERSEM_H_ */
