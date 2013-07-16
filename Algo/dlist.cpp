#include "list.h"
#include <iostream>

void dclist::initialize(){
	psentinel=new dlist_node(0);
	psentinel->next=psentinel;
	psentinel->prev=psentinel;
	lsize=0;
}

dclist::dclist(){
	initialize();
}

dclist::dclist(int* p,int* q){
	initialize();
	push_front(p,q);
}

dclist::dclist(int* p,int size){

}

dclist::dclist(const vector<int>& v){

}

dclist::~dclist(){
	//TODO
	printf("~dlist()\n");
}

/// @brief pass the end, which is the same as the sentinel in this case
dlist_node* dclist::end(){
	return psentinel;
}

dlist_node* dclist::begin(){
	return psentinel->next;
}

size_t dclist::size(){
	return lsize;
}

///@brief push new node as the next of sentinel
void dclist::push_front(int n){
	dlist_node* ln=new dlist_node(n);

	dlist_node* tmp=psentinel->next;

	psentinel->next=ln;
	ln->prev=psentinel;

	ln->next=tmp;
	tmp->prev=ln;

	lsize++;
}

void dclist::push_front(int* p,int* q){
	while (p!=q){
		push_front(*p++);
	}
}


/// @brief push new node as the previous of sentinel\n
/// A.next=B then B.prev=A, they always appear as a pair
void dclist::push_end(int n){
	dlist_node* ln=new dlist_node(n);

	dlist_node* tmp=psentinel->prev;

	psentinel->prev=ln;
	ln->next=psentinel;

	ln->prev=tmp;
	tmp->next=ln;

	lsize++;
};

void dclist::reverse(){
	dlist_node* tmp=psentinel;
	do{
		dlist_node* tmp_nxt=tmp->next;
		std::swap(tmp->next,tmp->prev);
		tmp=tmp_nxt;
	}while(tmp!=psentinel);
}

bool dclist::test(){
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	dclist dl(a,a+sizeof(a)/sizeof(int));
	//dl.push_front(a,a+sizeof(a)/sizeof(int));


	{
		dlist_node* tmp=dl.begin();
		while(tmp!=dl.end()){
			cout << tmp->d << endl;
			tmp=tmp->next;
		}

		cout << "\n" << endl;
	}

	dl.reverse();

	{
		dlist_node* tmp=dl.begin();
		while(tmp!=dl.end()){
			cout << tmp->d << endl;
			tmp=tmp->next;
		}

		cout << "\n" << endl;
	}

	{
		dlist_node* tmp=dl.end()->prev;
		while(tmp!=dl.end()){
			cout << tmp->d << endl;
			tmp=tmp->prev;
		}
	}
	return true;
}