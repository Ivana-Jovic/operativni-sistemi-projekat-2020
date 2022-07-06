/*
 * kersem.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */
#include "kersem.h"

List<KernelSem *> KernelSem::semaphores;
extern int syncPrintf (const char* format, ... );

//////////////////////pazi HARD LOCK VLAAL

KernelSem::KernelSem(int init) {
	//lock
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	semVal=init;//if(init<0)  init=0;
	semaphores.addEnd(this);
	//unlock
#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif
}

KernelSem::~KernelSem() {
	//lock
#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	//syncPrintf("delete kern sem");
	for(semaphores.onFirst();semaphores.currentExists();semaphores.onNext()){
		if((*(semaphores.getCurrent()))==this){
			//semaphores.getCurrent();
			semaphores.deleteCurrent();
			break;//jel sme?
		}
	}

	//ali ne zaboravi da odvezes
	for(waitingThreadsOnSem.onFirst();waitingThreadsOnSem.currentExists();waitingThreadsOnSem.onNext()){
		*waitingThreadsOnSem.getCurrent()=nullptr;
		//waitingThreadsOnSem.deleteFirst(); //zezne se for

	}
	waitingThreadsOnSem.deleteList();
	//unlock
#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif
}


void semaphoreTimeDecr() {
	//h_lock
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	//syncPrintf("KerSem-semaphoreTimeDecr");
	KernelSem* tmp=nullptr;

	for(KernelSem::semaphores.onFirst();KernelSem::semaphores.currentExists();KernelSem::semaphores.onNext()){
		KernelSem** t=KernelSem::semaphores.getCurrent();
		if(t==nullptr) continue;
		tmp=(*(KernelSem::semaphores.getCurrent()));

		for(tmp->waitingThreadsOnSem.onFirst();tmp->waitingThreadsOnSem.currentExists();){

			if(--(*(tmp->waitingThreadsOnSem.getCurrent()))->timeToWait==0){

				//syncPrintf("KerSem-semaphoreTimeDecr- unblock je\n");
				(*(tmp->waitingThreadsOnSem.getCurrent()))->unblockedBcTime=1;

				(*(tmp->waitingThreadsOnSem.getCurrent()))->status=READY;
				 Scheduler::put((PCB*)(*tmp->waitingThreadsOnSem.getCurrent()));

				 *(tmp->waitingThreadsOnSem.getCurrent())=nullptr;

				 tmp->waitingThreadsOnSem.deleteCurrent();////OVDE SE ZEZNE FOR tj preskoci se jedan i zato je korak pomeren

				 tmp->semVal++;
				// (*(KernelSem::semaphores.getCurrent()))->semVal++;
			}
			else tmp->waitingThreadsOnSem.onNext();

		}
	}
	//h_unlock
	#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif

}

int KernelSem::valKernel() const {
	return semVal;
}

int KernelSem::waitKernel(Time maxTimeToWait) {//Zadata vrednost 0 znaci da duzina cekanja nije vremenski ogranicena.
	#ifndef BCC_BLOCK_IGNORE//vla al
		asm pushf
		asm cli
	#endif
	//syncPrintf("waitKernel\n");
	semVal--;
	if((semVal)<0){
		//syncPrintf("waitKernel- inside\n");
		running->status=BLOCKED;
		running->timeToWait=maxTimeToWait;
		waitingThreadsOnSem.addEnd((PCB*)running);
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		dispatch();
	}
	else {
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
	}


	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	if(running->unblockedBcTime){
		running->unblockedBcTime=0;
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		return 0;
	}

	#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif
	return 1;
}












int KernelSem::signalKernel(int n) {
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	//syncPrintf("signalKernle\n");
	if(n==0){ //standard return 0;
		//syncPrintf("signalKernel - n==0\n");
		semVal++;
		if((semVal)<=0){
			PCB** pp=waitingThreadsOnSem.getFirst();
			if (pp==nullptr){
				#ifndef BCC_BLOCK_IGNORE
					asm popf
				#endif
				return 0;
			}

			PCB* p=*(waitingThreadsOnSem.getFirst());
			p->status=READY;
			p->unblockedBcTime=0;// zato sto ako je prvo time i pogodi se signal odmah posle nece biti ok vr
			(*(waitingThreadsOnSem.getFirst()))=nullptr;
			waitingThreadsOnSem.deleteFirst();
			Scheduler::put((PCB*)p);
		 }
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		 return 0;
	}



	if(n>0){
		//unblock n niti or if nblocked<n unblock all; sem val += nounbl?? or n; return nounb or n
		//syncPrintf("signalKernel - n>0\n");
		int noUnbl=0;
		if((0-semVal)<n) noUnbl=(0-semVal);
		else noUnbl=n;
		for(;noUnbl>0;noUnbl--){
			PCB** pp=waitingThreadsOnSem.getFirst();
			if (pp==nullptr){
				semVal+=n;
				#ifndef BCC_BLOCK_IGNORE
					asm popf
				#endif
				return noUnbl;
			}
			PCB* p=*(waitingThreadsOnSem.getFirst());
			p->status=READY;
			p->unblockedBcTime=0;
			*(waitingThreadsOnSem.getFirst())=nullptr;
			waitingThreadsOnSem.deleteFirst();
			Scheduler::put((PCB*)p);
		}
		//semVal+=noUnbl;
		semVal+=n;
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		return noUnbl;
	}



	if(n<0){
	//	syncPrintf("signalKernel - n<0\n");
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		return n;
	}


	return -1;//nece doci ovde nikad
}

