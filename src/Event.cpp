/*
 * event.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "Event.h"

extern void dispatch ();

Event::Event(IVTNo ivtNo) {
	lock
	myImpl= new KernelEv(ivtNo);
	unlock
}

Event::~Event() {
	lock
	if(myImpl) delete myImpl;
	unlock
}

void Event::wait() {
	lock
	myImpl->wait();
	unlock
}

void Event::signal() {
	lock
	myImpl->signal();
	unlock
}
