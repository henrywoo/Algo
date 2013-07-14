#include "bst.h"
#include <assert.h>
#include <iostream>
#include <stack>
#include <set>
#include <iterator>

bst::bst():proot(NULL){
}

bst::~bst(){
	//TODO
}

// @
void bst::insert(int n){
	cout << "insert " << n << endl;
	bstnode* p=new bstnode(n);
	if(proot==NULL){
		proot=p;
	}else{
		bstnode* tmp=proot;
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


vector<int> bst::walk_from_node(bstnode* p, WALKORDER wo, bool recursive){
	vector<int> v;
	if(wo==INORDER){
		if (recursive){
			if(p->l){
				vector<int> v2=walk_from_node(p->l,wo,recursive);
				v.insert(v.end(),v2.begin(),v2.end());
			}
			v.push_back(p->d);
			if(p->r){
				vector<int> v2=walk_from_node(p->r,wo,recursive);
				v.insert(v.end(),v2.begin(),v2.end());
			}
		}else{
			//iteratively
		}
	}else if (wo==POSTORDER){
		if (recursive)
		{
			if(p->l){
				vector<int> v2=walk_from_node(p->l,wo,recursive);
				v.insert(v.end(),v2.begin(),v2.end());
			}
			if(p->r){
				vector<int> v2=walk_from_node(p->r,wo,recursive);
				v.insert(v.end(),v2.begin(),v2.end());
			}
			v.push_back(p->d);
		}
	}
	return v;
}

vector<int> bst::walk(WALKORDER wo, bool recursive){
	vector<int> v;
	if(wo==PREORDER){
		if (recursive){

		}else{
			// the first one must be the root
			stack<bstnode*> stk;
			bstnode*tmp=proot;
			while(tmp){
				v.push_back(tmp->d);
				stk.push(tmp);
				if (tmp->l){
					tmp=tmp->l;
				}else{
					// look for the first right child of the nodes in stack - tmp2
					tmp=NULL;
					while (!tmp)
					{
						if (!stk.empty()){
							tmp=stk.top()->r;
							stk.pop();
						}else{
							tmp=NULL;break; // break the two while loops
						}
					}
				}
			}
		}
	}else if(wo==INORDER){
		if (recursive){
			v=walk_from_node(proot,wo,recursive);
		}else{
			//iteratively
			stack<bstnode*> stk;
			bstnode* tmp=proot;
			while(tmp){
				if (tmp->l){
					stk.push(tmp);
					tmp=tmp->l;
				}else{
					if (tmp->r){
						tmp=tmp->r;
					}else{
						v.push_back(tmp->d);

						if (stk.empty()){
							break;
						}
STACKPOP:
						bstnode*& bn=stk.top();
						// Be aware that the node popped out from the stack has got 
						// left children processed, so we only need to check the right children
						// print it when popping out
						v.push_back(bn->d);
						stk.pop();

						if (bn->r){
							tmp=bn->r;
						}else{
							if (stk.empty()){
								tmp=NULL;
							}else{
								goto STACKPOP;
							}
						}
					}
				}
			}
		}
	}else if(wo==POSTORDER){
		if (recursive){
			v=walk_from_node(proot,POSTORDER,true);
		}else{
			// the key point is
			//(1)what is inside the stack  left->left->left
			//(2)when to print the node[push/pop] 
			// the last one must be the root
			stack<bstnode*> stk;//ditto
			set<bstnode*> processed_nodes; // processed but not printed
			bstnode*tmp=proot;//ditto
			bool pushmode=true;
			while (tmp){
				if(pushmode){
					stk.push(tmp);
					processed_nodes.insert(tmp);
					if (tmp->l){
						tmp=tmp->l;
					}else if (tmp->r){
						tmp=tmp->r;
					}else{
						//leaf
						stk.pop();
						pushmode=false;
						//cout<< tmp->d << endl; PRINT WHEN POPPING OUT
						v.push_back(tmp->d);
						if (!stk.empty()){tmp=stk.top();}else{break;}
					}
				}else{
					if (tmp->r && processed_nodes.count(tmp->r)==0){
						tmp=tmp->r;
						pushmode=true;
					}else{
						stk.pop();
						//cout<< tmp->d << endl;
						v.push_back(tmp->d);
						if (!stk.empty()){tmp=stk.top();}else{break;}
						
					}
				}
			}
		}
		

	}else if(wo==LAYERBYLAYER){

	}

	return v;
}



bool isBST(const bst& t){
	return true;
}
WALKORDER getOrder(const vector<int>& v){
	return NONE;
}