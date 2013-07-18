#include "bst.h"
#include <assert.h>
#include <iostream>
#include <stack>
#include <deque>
#include <set>
#include <iterator>

bst::bst(){
}

bst::~bst(){
	//TODO
}

/// @
void bst::insert(int n){
	//cout << "insert " << n << endl;
	btnode* p= new (nothrow) btnode(n);
	if(proot==NULL){
		proot=p;
	}else{
		btnode* tmp=proot;
		while(tmp){
			if(n>tmp->d){
				if(tmp->r)
					tmp=tmp->r;
				else{
					tmp->r=p;
					break;
				}
			}else{
				if(tmp->l)
					tmp=tmp->l;
				else
				{
					tmp->l=p;
					break;
				}
			}
		}
	}
}

void bst::insert(int* p,int* q){
	assert(q>p);
	while(p<=q){
		insert(*p);
		++p;
	}
}

void bst::insert(int* p,int size){
	int tmp=0;// not tmp=0
	while(tmp<size){
		insert(*p);
		tmp++;
		p++;
	}	
}
void bst::insert(const vector<int>& v){
	for(vector<int>::const_iterator i=v.begin();i!=v.end();++i){
		insert(*i);
	}
}


vector<int> bst::walk_from_node(btnode* p, WALKORDER wo) const{
	vector<int> v;
	if(wo==INORDER){
		if(p->l){
			vector<int> v2=walk_from_node(p->l,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
		v.push_back(p->d);
		if(p->r){
			vector<int> v2=walk_from_node(p->r,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
	}else if (wo==POSTORDER){
		if(p->l){
			vector<int> v2=walk_from_node(p->l,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
		if(p->r){
			vector<int> v2=walk_from_node(p->r,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
		v.push_back(p->d);
	}else if (wo==PREORDER){
		v.push_back(p->d);
		if(p->l){
			vector<int> v2=walk_from_node(p->l,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
		if(p->r){
			vector<int> v2=walk_from_node(p->r,wo);
			v.insert(v.end(),v2.begin(),v2.end());
		}
	}
	return v;
}

void helper_pushallleftsidenodes(btnode* b, stack<btnode*>& s){
	while(b){
		s.push(b);
		b=b->l;
	}
}
void helper_pushallleftsidenodes(btnode* b, stack<btnode*>& s, vector<int>& v){
	while(b){
		s.push(b);
		v.push_back(b->d);
		b=b->l;
	}
}
#if 0
void helper_pushallrightsidenodes(bstnode* b, stack<bstnode*>& s){
	while(b){
		s.push(b);
		b=b->r;
	}
}
#endif
void helper_pushallrightsidenodes(btnode* b, stack<btnode*>& s, vector<int>& v){
	while(b){
		s.push(b);
		v.push_back(b->d);
		b=b->r;
	}
}

///@brief POSTORDER is more complex than the other two traversals
///(due to its nature of non-tail recursion there is an extra statement
///after the final recursive call to itself).
vector<int> bst::walk(WALKORDER wo, bool norecursive) const
{
	vector<int> v;
	if(wo==PREORDER){
		if (norecursive){
			// the first one must be the root
			stack<btnode*> stk;
			btnode*tmp=proot;
			helper_pushallleftsidenodes(proot,stk,v);
			while (!stk.empty()){
				btnode* tmp=stk.top();
				//v.push_back(tmp->d);
				stk.pop();
				if (tmp->r){
					helper_pushallleftsidenodes(tmp->r,stk,v);
				}
			}
		}else{
			v=walk_from_node(proot,wo);
		}
	}else if(wo==INORDER){
		if (norecursive){
			//iteratively
			stack<btnode*> stk;
			helper_pushallleftsidenodes(proot,stk);
			while (!stk.empty()){
				btnode* tmp=stk.top();
				v.push_back(tmp->d);
				stk.pop();
				if (tmp->r){
					helper_pushallleftsidenodes(tmp->r,stk);
				}
			}
		}else{
			v=walk_from_node(proot,wo);
		}
	}else if(wo==POSTORDER){
		if (norecursive){
			// http://www.geeksforgeeks.org/iterative-postorder-traversal/
			stack<btnode*> stk;
			btnode*tmp=proot;
			helper_pushallrightsidenodes(proot,stk,v);
			while (!stk.empty()){
				btnode* tmp=stk.top();
				//v.push_back(tmp->d);
				stk.pop();
				if (tmp->l){
					helper_pushallrightsidenodes(tmp->l,stk,v);
				}
			}
			reverse(v.begin(),v.end());
		}else{
			v=walk_from_node(proot,POSTORDER);
		}
	}else if(wo==LAYERBYLAYER){
		deque<btnode*> dbn;
		dbn.push_back(proot);
		while (!dbn.empty()){
			btnode* tmp=*dbn.begin();
			v.push_back(tmp->d);
			dbn.pop_front();
			if(tmp->l)
				dbn.push_back(tmp->l);
			if(tmp->r)
				dbn.push_back(tmp->r);
		}
	}else if (wo==ZIGZAG){
		stack<btnode*> stk1;
		stack<btnode*> stk2;
		stack<btnode*> *sp1=&stk1,*sp2=&stk2;
		sp1->push(proot);
		bool left2right=true;
		while (true){
			btnode* tmp=sp1->top();
			v.push_back(tmp->d);
			sp1->pop();

			if (left2right){
				if (tmp->r){sp2->push(tmp->r);}
				if (tmp->l){sp2->push(tmp->l);}
			}else{
				if (tmp->l){sp2->push(tmp->l);}
				if (tmp->r){sp2->push(tmp->r);}
			}
			if (sp1->empty()){
				if (!sp2->empty()){
					swap(sp1,sp2);
					left2right=!left2right;
				}else{
					break;
				}
			}
		}
	}
	return v;
}

bool bst::isBST(const bst& t){
	vector<int> v=t.walk(INORDER);
	int tmp=INT_MIN;
	for (vector<int>::const_iterator i=v.begin();i!=v.end();++i)
	{
		if (tmp<*i)
		{
			tmp=*i;
		}else
			return false;
	}
	return true;
}

WALKORDER bst::getOrder(const vector<int>& v){
	return NONE;
}

vector<int> bst::getdepth(int n){
	vector<int> v;
	return v;
}

bool bst::test(){
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	vector<int> v2(a,a+sizeof(a)/sizeof(int));
	for (vector<int>::reverse_iterator ri=v2.rbegin();ri!=v2.rend();++ri){
		cout << *ri << endl;
	}
	bst t;
	t.insert(a,sizeof(a)/sizeof(int));

	/*
	10 5 7 6 9 8 40 25 13 21 16 19 23 50
	5 6 7 8 9 10 13 16 19 21 23 25 40 50
	6 8 9 7 5 19 16 23 21 13 25 50 40 10
	*/

	vector<int> v=t.walk(PREORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout,","));
	cout << "\n";

	v=t.walk(INORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	
	v=t.walk(POSTORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout,","));
	cout <<'\b'<< endl;

	v=t.walk(LAYERBYLAYER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;

	reverse(v.begin(),v.end());
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;

	v=t.walk(ZIGZAG);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;

	bst::isBST(t);
	return true;
}


void heap::print(){
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
}

bool heap::test(){
	heap h;
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	int* b=a;
	while(b!=a+sizeof(a)/sizeof(int)){
		h.insert(*b);
		b++;
	}
	h.print();
	return true;
}

void heap::insert(int n){
	v.push_back(n);
	int pos=v.size()-1;
	while (v.at(PARENTPOS(pos))<n){
		int tmp=v.at(PARENTPOS(pos));
		v.at(PARENTPOS(pos))=n;
		v.at(pos)=tmp;
		if (pos==0){
			break;
		}
		pos=PARENTPOS(pos);
	}
}










//////////////////////////////////////////////////////////////////////////
namespace sandbox{
template<class T>
void heap<T>::insert(const T& n){
	cout << n << endl;
}


/// No need to call this TemporaryFunction() function,
/// it's just to avoid link error.
#include <assert.h>
void TemporaryFunction ()
{
	assert(1);
	heap<double> TempObj;
	TempObj.insert(.0);
}
}