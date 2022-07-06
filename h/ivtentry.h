/*
 * ivtentry.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "const.h"
#include "kerev.h"

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine)(...);

#define PREPAREENTRY(num, flagCallOldRout)\
    void interrupt intr##num(...);\
    IVTEntry ivtE##num(num, &intr##num);\
    void interrupt intr##num(...){\
        if(flagCallOldRout) ivtE##num.myOldIntrR();\
		ivtE##num.myKEvent->signal();\
		dispatch();\
    }


//  IVTEntry ivtE##num(num, intr##num);

class KernelEv;


class IVTEntry{
public:
	static IVTEntry* IVTable[256];
	IVTEntry(IVTNo n, InterruptRoutine i);
	~IVTEntry();

	IVTNo myIvtNo;
	InterruptRoutine myOldIntrR, myNewIntrtR;
	KernelEv* myKEvent;
	static IVTEntry* getEntry(IVTNo n);
	void setKEvent(KernelEv* k);
};


#endif


