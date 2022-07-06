/*
 * userth.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */

#include "userth.h"


UserThread::UserThread(int argc1, char** argv1): Thread(){
	argc=argc1;
	argv=argv1;
	retCode =0;
/*	lock
	cout<<"USERTHREAD KONSTRUKTOR \n";
	unlock*/
}

int UserThread::returnCode() {
	return retCode;
}
UserThread::~UserThread(){
	waitToComplete();

}
void UserThread::run(){
	/*lock
	cout<<"USERTHREAD RUN PRE USRMAIN\n";
	unlock*/
	retCode=userMain(argc,argv);
	/*lock
	cout<<"USERTHREAD RUN POSLE USRMAIN\n";
	unlock*/
}
