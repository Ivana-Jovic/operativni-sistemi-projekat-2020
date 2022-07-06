/*
 * kerev.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "kerev.h"

KernelEv::KernelEv(IVTNo n) {
	lock
	ivtNo=n;
	myKSem= new KernelSem(0);
	if(running!= Kernel::getIdlePCB()) myPcb=(PCB*)running;// ima li potrebe za ovim?
	IVTEntry::IVTable[n]->setKEvent(this);//ili IVTEntry::getEntry(ivtNo)->setKEvent(this);
	unlock
}

KernelEv::~KernelEv() {
	myPcb=nullptr;
	lock delete myKSem; unlock
	IVTEntry::getEntry(ivtNo)->setKEvent(nullptr);// da li treba ekspl pozvati destr ivtentry- ne

}

void KernelEv::wait() {
	/* obezbediti da samo nit koja je kreirala
	dogadjaj moze na njemu i da se blokira; poziv ove operacije iz niti koja nije vlasnik
	dogadjaja nema efekta; */
	//lock
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif
	if(myKSem->semVal==-1){
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		return;
	}
	if((myPcb== running)){
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		myKSem->waitKernel(0);
		//unlock
	//myKSem->semVal=0;//!!!!!!!!!!!!!!!1
	}
	else{ //unlock
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
	}

}

void KernelEv::signal() {
	/* i koju treba da pozove
prekidna rutina koja je vezana za prekid dogadjaja, a koju obezbedjuje korisnik;
preciznije, korisnicka prekidna rutina koja je vezana za isti ulaz u IVT za koji je vezan
i dogadjaj, treba samo da pozove ovu operaciju signal dogadjaja koji odgovara tom
prekidu.
*/
//	lock
	#ifndef BCC_BLOCK_IGNORE
		asm pushf
		asm cli
	#endif

	if(myKSem->semVal==1){
		#ifndef BCC_BLOCK_IGNORE
			asm popf
		#endif
		return;
	}
	 myKSem->signalKernel(0);
		//myKSem->semVal=1;//!!
	// }
	//unlock
	#ifndef BCC_BLOCK_IGNORE
		asm popf
	#endif
}



