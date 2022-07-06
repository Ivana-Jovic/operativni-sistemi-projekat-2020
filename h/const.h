/*
 * const.h
 *
 *  Created on: Apr 28, 2020
 *      Author: OS1
 */

#ifndef CONST_H_
#define CONST_H_


#define nullptr 0
#define true 1
#define false0

/*#define h_lock
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
		//asm pushf; asm cli;
#define h_unlock
		#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif
*/
//pazi ovako disp da se ne poziva iz zaklnjucanog dela
extern int volatile lockCnt;//static?
extern int volatile lockTimedOut;

#define Locks    ++lockCnt
#define Unlocks   if ((--lockCnt) == 0 && lockTimedOut==1) { dispatch(); }

#define lock   Locks;
#define unlock    Unlocks;

#endif /* CONST_H_ */
