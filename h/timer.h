/*
 * timer.h
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "thread.h"
#include <dos.h>
#include "const.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "kernel.h"
#include <iostream.h>
//#include <dos.h>
#include "kersem.h"

void interrupt timer(...);
extern void tick();

class Timer{
	Time quantum;
public:
	typedef void interrupt (*oldTimer)();

	//	unsigned oldTimerOFF, oldTimerSEG;
	typedef void interrupt (*pInterrupt)(...);
	static pInterrupt oldIntr;

	static	 volatile int context_switch_on_demand;
	static	 volatile int cnt;

	 //volatile int tsp,tss,tbp;
	//public:static int volatile lockTimedOut;

	static void inic();
	static void restore();



};


#endif /* TIMER_H_ */
