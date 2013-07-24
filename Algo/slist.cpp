#include "list.h"
#include <iostream>

slist::slist():head(NULL){}

slist::slist(int* p,int* q):head(NULL){
	while (p!=q){
		push(*p++);
	}
}
slist::slist(int* p,int size):head(NULL){
	while (size-->=0){
		push(*p++);
	}
}
slist::slist(const vector<int>& v){}

slist::~slist(){
	slist_node* tmp=NULL;
	while(head->next!=NULL){
		tmp=head;
		head=head->next;
		delete tmp;
	}
}

// the original head was pushed to the rear
void slist::push(int n){
	if (!head){
		head=new slist_node(n);
		head->next=NULL;
	}else{
		slist_node* tmp=head;
		head=new slist_node(n);
		head->next=tmp;
	}
	this->lsize++;
}

void slist::push(int* p,int* q){}

slist_node* slist::end(){
	//return tail->next;
	return NULL;
}

slist_node* slist::begin(){
	return head;
}

bool slist::test(){
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	slist sl(a,a+(sizeof(a)/sizeof(int)>>1));
	int j=(sizeof(a)/sizeof(int)-1);
	for (int i=-1;i<=j;++i){// test case 1
		const slist_node* p=sl.FindKthToTail(i);
		cout << hex << p << "\t" << (p?p->d:0) << endl;
	}

#if 0
    for(slist_node* i=sl.begin();i!=sl.end();i=i->next){
		cout << i->d << endl;
	}
#endif

	// The paradigm of loop a slist
	slist_node* i=sl.begin();
	slist_node* prevnode=NULL;// can be assigned to i, it doesnt matter
	while(i!=NULL){
		prevnode=i;
		i=i->next;
	}
	cout << "Last node:" << prevnode->d << endl;
	return true;
}

