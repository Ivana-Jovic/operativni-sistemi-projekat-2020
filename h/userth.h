/*
 * mainth.h
 *
 *  Created on: Jul 30, 2020
 *      Author: OS1
 */

#ifndef USERTH_H_
#define USERTH_H_

#include "thread.h"
#include "const.h"
#include <iostream.h>

extern int userMain(int argc, char* argv[]);// OVDE JE BILO DUPLICTATE SVEGA I SVACEGA I RESENO JE TAKO SO JE DODATO OVO
//A INCL TEST2.CPP ZAKOMENTARISANO

class UserThread: public Thread{
	int argc;
	char **argv;
	int retCode;
public:
	UserThread(int argc1,char ** atgv1);
	int returnCode();
	~UserThread();
	void run();
};

#endif /* USERTH_H_ */
