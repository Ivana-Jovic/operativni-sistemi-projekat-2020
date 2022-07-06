/*
 * idleth.h
 *
 *  Created on: Jul 27, 2020
 *      Author: OS1
 */

#ifndef IDLETH_H_
#define IDLETH_H_

#include "thread.h"
#include <iostream.h>
#include "const.h"


extern int syncPrintf (const char* format, ... );

class IdleThread : public Thread {
public:
	IdleThread():Thread(65536, 1) {// :Thread(65536, 1){ ili 64,1
		////*Kernel::getIdlePCB()->id=0;*/
		//syncPrintf("IDLE NIT KONSTRUKTOR\n");
	}
protected:
	//virtual void run();
	 void run(){
		while(1){}
	 }

};
#endif /* IDLETH_H_ */
