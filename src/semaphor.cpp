/*
 * semaphor.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */
#include "semaphor.h"
#include "kersem.h"
#include "const.h"

extern void dispatch ();


Semaphore::Semaphore(int init) {
	lock
	myImpl= new KernelSem(init);
	unlock
}

Semaphore::~Semaphore() {//mozda celo u lock
	if(myImpl){//!=null
		lock
		delete myImpl;
		unlock
		myImpl=nullptr;
	}
}

int Semaphore::wait(Time maxTimeToWait) {
	lock
	int ret=myImpl->waitKernel(maxTimeToWait);
	unlock
	return ret;
}

int Semaphore::signal(int n) {
	lock
	int ret=myImpl->signalKernel(n);
	unlock
	return ret;

}

int Semaphore::val() const {
	lock
	int ret= myImpl->valKernel();
	unlock
	return ret;

}

/*Semaphore::Semaphore(int init) {
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	myImpl= new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
asm popf
#endif
}

Semaphore::~Semaphore() {//mozda celo u lock
	if(myImpl){//!=null
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
		delete myImpl;
#ifndef BCC_BLOCK_IGNORE
asm popf
#endif
		myImpl=nullptr;
	}
}

int Semaphore::wait(Time maxTimeToWait) {
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	int ret=myImpl->waitKernel(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
asm popf
#endif
	return ret;
}

int Semaphore::signal(int n) {
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	int ret=myImpl->signalKernel(n);
#ifndef BCC_BLOCK_IGNORE
asm popf
#endif
	return ret;

}

int Semaphore::val() const {
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
		int ret= myImpl->valKernel();
#ifndef BCC_BLOCK_IGNORE
asm popf
#endif
	return ret;

}*/
