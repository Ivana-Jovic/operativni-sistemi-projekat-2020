/*
 * kerev.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef KEREV_H_
#define KEREV_H_
#include "kersem.h"
#include "const.h"
#include "kernel.h"
#include "ivtentry.h"
#include "pcb.h"

typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv (IVTNo n);
	~KernelEv ();
	void wait ();
	void signal();

	IVTNo ivtNo;

	KernelSem* myKSem; //ili volatile int value;
	PCB* myPcb;

};


#endif /* KEREV_H_ */
