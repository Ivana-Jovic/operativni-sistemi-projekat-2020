/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include "kernel.h"
#include "userth.h"

extern int syncPrintf (const char* format, ... );


int main(int argc, char** argv)
{
	syncPrintf( "----1-----Hello World1!\n");
	Kernel::inic();
	//syncPrintf( "----2-----Hello World1!\n");
	lock
	UserThread* userTh=new UserThread(argc, argv);
	unlock
	userTh->start();
	userTh->waitToComplete();
	int ret = userTh->returnCode();


	lock
	delete userTh;
	unlock
	//delete idle....
//	syncPrintf( "----3-----Hello World1!\n");

	Kernel::restore();

	syncPrintf( "----4-----Hello World1!\n");


	return ret;
}

