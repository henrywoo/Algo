#include "bst.h"
#include <assert.h>
#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include <iterator>
#include "util.h"

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
#define GEEK
#ifdef GEEK
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
#else
			stack<btnode*> stk;
			btnode*tmp=proot;
			helper_pushallleftsidenodes(proot,stk);
			while (!stk.empty())
			{
				btnode* tmp=stk.top();
				stk.pop();
				if (tmp)
				{
				}
			}
#endif
		}else{
			v=walk_from_node(proot,POSTORDER);
		}
	}else if(wo==LAYERBYLAYER){
#if 0
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
#else
		queue<btnode*> dbn;
		dbn.push(proot);
		while (!dbn.empty()){
			btnode* tmp=dbn.front();
			v.push_back(tmp->d);
			dbn.pop();
			if(tmp->l)
				dbn.push(tmp->l);
			if(tmp->r)
				dbn.push(tmp->r);
		}
#endif
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

///@todo could be optimized
bool bst::isBST(const bst& t){
	vector<int> v=t.walk(INORDER);
	int tmp=INT_MIN;
	for (vector<int>::const_iterator i=v.begin();i!=v.end();++i){
		if (tmp<*i){
			tmp=*i;
		}else{
            return false;
        }
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

size_t bst::diameter(){
    return diameter(proot);
}

size_t bst::height(){
    return height(proot);
}

size_t bst::height(btnode* pn){//inclusive
    if (pn==NULL){
        return 0;
    }
    if (pn->l==NULL && pn->r==NULL){
        return 1;
    }
    int lh=pn->l?height(pn->l):0;
    int rh=pn->r?height(pn->r):0;
    return max(lh,rh);
}

size_t bst::diameter(btnode* pn){//inclusive
    if (pn==NULL){
        return 0;
    }else{
        if (pn->l==NULL && pn->r==NULL){
            return 1;
        }else{
            int ld=pn->l?diameter(pn->l):0;
            int rd=pn->r?diameter(pn->r):0;

            int lh=pn->l?height(pn->l):0;
            int rh=pn->r?height(pn->r):0;

            return max(lh+rh+1,max(ld,rd));
        }
    }
}

bst_iterator_postorder::bst_iterator_postorder(bst& lhs):
    t(lhs),
    p2curr_node(NULL),
    pushmode(true)
{
    //p2curr_node=NULL;
    //pushmode=true;
    findfirstnode();
}
bst_iterator_postorder::~bst_iterator_postorder(){}

void bst_iterator_postorder::reset(){
    p2curr_node=NULL;
    pushmode=true;
    processed_nodes.clear();
    findfirstnode();
    //assert(stk.empty());
}

void bst_iterator_postorder::findfirstnode(){
    p2curr_node=t.getroot();
    while(true){
        stk.push(p2curr_node);
        processed_nodes.insert(p2curr_node);
        if (p2curr_node->l){
            p2curr_node=p2curr_node->l;
        }else if (p2curr_node->r){
            p2curr_node=p2curr_node->r;
        }else{
            stk.pop();
            pushmode=false;
            break;
        }
    }
}

btnode* bst_iterator_postorder::next()
{
    p2curr_node=stk.empty()?NULL:stk.top();
    while(p2curr_node){
        if(pushmode){
            stk.push(p2curr_node);
            processed_nodes.insert(p2curr_node);
            if (p2curr_node->l){
                p2curr_node=p2curr_node->l;
            }else if (p2curr_node->r){
                p2curr_node=p2curr_node->r;
            }else{
                //leaf
                stk.pop();
                pushmode=false;
                return p2curr_node;
            }
        }else{
            if (p2curr_node->r && processed_nodes.count(p2curr_node->r)==0){
                p2curr_node=p2curr_node->r;
                pushmode=true;
            }else{
                stk.pop();
                return p2curr_node;
            }
        }
    }
    if(!p2curr_node) reset();//??
    return NULL;
}

bst_iterator_postorder& bst_iterator_postorder::operator++(){
    p2curr_node=next();
    return *this;
}

///post-autoincrement
bst_iterator_postorder bst_iterator_postorder::operator++(int){
    bst_iterator_postorder ti=*this;
    ++*this;//++ti;
    return ti;
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
    cout <<"POST:"<< endl;
	copy(v.begin(),v.end(),ostream_iterator<int>(cout,","));
	cout << endl;

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

	//bst t;
	bst_iterator_postorder bip(t);//equivalent to v.begin()
#if 0
	btnode* p=bip.begin();
	while(p){
		cout << p->d << endl;
		p=bip.next();
        //++p;
	}
#endif

    while (bip){
        cout << *bip << endl;
        //bip.next();
        ++bip;
    }

	return true;
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