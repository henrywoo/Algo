#include "list.h"
#include <iostream>

slist::slist():head(NULL),tail(NULL){}

slist::slist(int* p,int* q):head(NULL),tail(NULL){
	while (p!=q){
		push_front(*p++);
	}
}
slist::slist(int* p,int size):head(NULL),tail(NULL){
	while (size-->=0){
		push_front(*p++);
	}
}
slist::slist(const vector<int>& v){}

slist::~slist(){
}

//
void slist::push_front(int n){
	if (!head){
		head=new slist_node(n);
		tail=head;
	}else{
		slist_node* tmp=head;
		head->next=new slist_node(n);
		head->next->next=tmp;
	}
}

void slist::push_front(int* p,int* q){}

slist_node* slist::end(){
	return tail->next;
}

slist_node* slist::begin(){
	return head;
}

bool slist::test(){
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	slist sl(a,a+sizeof(a)/sizeof(int));

	for(slist_node* i=sl.begin();i!=sl.end();i=i->next){
		cout << i->d << endl;
	}

	// The paradigm of loop a slist
	slist_node* i=sl.begin();
	slist_node* lastnode=NULL;// can be assigned to i, it doesnt matter
	do {
		///
		lastnode=i;
		i=i->next;
	} while (i->next);
	return true;
}

