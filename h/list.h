/*
 * list.h
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "const.h"
#include <stdlib.h>
#include <iostream.h>

extern int syncPrintf (const char* format, ... );
template <class T>

class List{
public:
	List(){ head=tail=current=beforeCurrent=nullptr; n=0;}
	//virtual ~List();
	struct Elem{
		T data;
		Elem * next;
		Elem(const T& d, Elem * n=nullptr): data(d), next(n){}
	//	Elem(T d, Elem * n=nullptr): data(d), next(n){}
	};

	Elem *head, *tail,*current, *beforeCurrent;
	int n;


	void deleteList(){
		lock
		Elem * tmp= head;
		while(head){
			tmp=head->next;
			lock
			delete head;
			unlock
			head= tmp;
		}
		n=0;
		head=tail=current=beforeCurrent=nullptr;
		unlock
	}
	List& addEnd(const T& d){
		lock
		tail=(!head?head:tail->next)=new Elem(d);
		n++;
		unlock
		return *this;
	}
//ovako se vanklase :template<class T>
	//void Lista<T>::
	List& onFirst(){
		lock
		current = head;
		beforeCurrent = nullptr;
		unlock
		return *this;
	}
	List& onNext(){
		lock
		beforeCurrent = current;
		if (current) current = current->next;
		unlock
		return *this;
	}
	int currentExists() const{
		lock
		if (current != nullptr) {unlock return 1;}
		 else {
			 //lock cout<<"ne postoji tekuci"; unlock
			 unlock
			 return 0;
		 }
	}

	T* getCurrent(){//T& getCurrent(){...return current->data;
			lock
			if(!currentExists()) {syncPrintf("ne postoji curr"); unlock return nullptr;}
			unlock
			return &current->data;
			//else return 0;
			//else {lock cout<<"ne postoji tekuci"; unlock}
		}
	T* getFirst(){//T& getFirst(){return head->data;....//ovo je izmena 17.08.20:30
		lock
			if(!head) { syncPrintf("ne postoji prvi"); unlock return nullptr;}
		unlock

		return &head->data;

	}

	void deleteFirst(){
		lock
		onFirst();
		deleteCurrent();
		unlock
	}
	void deleteCurrent(){
		lock
		if(current){//if(currentExists()){
			Elem* old = current;
			current= current->next;
			(!beforeCurrent ? head : beforeCurrent->next) = current;

			if (!current) tail = beforeCurrent;

			lock
			delete old;
			//unlock
			old=nullptr;//? jedina zimena 19.08
			n--;
			unlock
			unlock

		}
		else unlock
	}
};



#endif /* LIST_H_ */
