/*
 * ivtentry.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "ivtentry.h"
#include <dos.h>

extern void dispatch ();
IVTEntry* IVTEntry::IVTable[256] = { 0 };

IVTEntry::IVTEntry(IVTNo n, InterruptRoutine i) {
	lock
	myIvtNo=n;
	myNewIntrtR=i;
	IVTable[myIvtNo]=this;
	myKEvent=nullptr;
	unlock
}

IVTEntry::~IVTEntry() {
	lock
// call oldroutine
	#ifndef BCC_BLOCK_IGNORE
		setvect(myIvtNo,myOldIntrR);
	#endif
	myOldIntrR=myOldIntrR=0;
	IVTable[myIvtNo]=nullptr;
	myKEvent=nullptr;
	unlock
}


IVTEntry* IVTEntry::getEntry(IVTNo n) {
	return IVTable[n];
}


void IVTEntry::setKEvent(KernelEv* k) {
	lock

	myKEvent=k;

	#ifndef BCC_BLOCK_IGNORE
		myOldIntrR=getvect(myIvtNo);
		setvect(myIvtNo,myNewIntrtR);
	#endif
	unlock
}
