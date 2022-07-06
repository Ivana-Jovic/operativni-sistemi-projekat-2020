/*
 * timer.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "timer.h"


typedef void interrupt (*pInterrupt)(...);
pInterrupt Timer::oldIntr=0;
/*unsigned Timer::oldTimerOFF=0, Timer::oldTimerSEG=0;
volatile int Timer::context_switch_on_demand=0;
volatile int Timer::cnt=20;
volatile int Timer::tsp=0,Timer::tss=0,Timer::tbp=0;
*/
static unsigned oldTimerOFF, oldTimerSEG;
volatile int Timer::context_switch_on_demand=0;
volatile int Timer::cnt=20;//koja vrednost da se stavi?
static volatile int tsp,tss,tbp;

extern int syncPrintf (const char* format, ... );

//int volatile Timer::lockTimedOut =0;// false;

void Timer::inic(){
/*	lock
		cout<<"u inic timeru\n"<<endl;
		unlock*/
/*	lock
	asm{
		//cli
		push ax
		push es
		mov ax,0
		mov es, ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax


		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0180h, ax

		pop es
		pop ax
	//	sti
	}
	unlock*/
#ifndef BCC_BLOCK_IGNORE
		oldIntr=getvect(0x08);
		setvect(0x08,timer);
		#endif

}
void Timer::restore(){
/*
	lock
		cout<<"u restore  timeru\n"<<endl;
		unlock
	lock
	asm{
//			cli
			push ax
			push es
			mov ax,0
			mov es, ax

			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov	ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop es
			pop ax
	//		sti
		}
	unlock*/
#ifndef BCC_BLOCK_IGNORE
		setvect(0x08,oldIntr);
		#endif
}

void interrupt timer(...){
	//syncPrintf("1\n");


	if(!Timer::context_switch_on_demand) {
			//syncPrintf("u timeru pre tick\n");
			tick();
			(*Timer::oldIntr)();
			/*#ifndef BCC_BLOCK_IGNORE
					asm {
					int 60h; //za staru
					}
			#endif*/
			semaphoreTimeDecr();
		}

	if(Timer::context_switch_on_demand ==0) {
		Timer::cnt--;
		if(Timer::cnt==0 && lockCnt==1) lockTimedOut =1; //true;
	}
	else if (lockTimedOut) {
		lockTimedOut = 0;//false;
	}




	if((Timer::cnt==0 && !lockCnt) || Timer::context_switch_on_demand){
		asm{
				/*push ax// ovo je automatski zbog interrupt
				push bx
				push cx
				push dx*/
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
				running->sp=tsp;
				running->ss=tss;
				running->bp = tbp;
				running->savedLock=lockCnt;// ovo je u const.h
			if(running->status == READY && running!= Kernel::getIdlePCB()) {
				Scheduler::put((PCB*)running);
			//	syncPrintf("u timeru nakon put\n");

			}

			running= Scheduler::get();


				if(running==nullptr){
					running = Kernel::getIdlePCB();
					//syncPrintf("....u timeru uzet idle\n");
					}

				tsp=running-> sp;
				tss=running-> ss;
				tbp=running->bp;
				Timer::cnt=running->timeSlice;

			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
				/*pop dx
				pop cx
				pop bx
				pop ax*/
				}
			lockCnt= running->savedLock;
	}

	//if(!Timer::context_switch_on_demand)asm int 60h;// ovo je sad gore izmenjeno
	/*if(!Timer::context_switch_on_demand){
		#ifndef BCC_BLOCK_IGNORE
					asm {
					int 60h; //za staru
					}
					#endif
	}*/


	Timer::context_switch_on_demand=0;// false;

}
