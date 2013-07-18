#ifndef __ALGO_BST__
#define __ALGO_BST__

#include <vector>
#include <stack>
#include <set>

using namespace std;

enum WALKORDER{
	PREORDER=1, /// head->left->right
	INORDER,    /// left->head->right
	POSTORDER,  /// left->right->head
	REVERSE_PREORDER, /// right->left->head
	REVERSE_INORDER,  /// right->head->left
	REVERSE_POSTORDER,/// head->right->left
	LAYERBYLAYER, //layer by layer
	ZIGZAG,
	NONE=-1 //
};

///@brief arbitrary binary tree node[bst,heap,etc]
struct btnode{
	btnode* l;// left node
	btnode* r;// right node
	int d;
	short color;
	btnode* par;
	btnode(int n):l(NULL),r(NULL),d(n),color(0){}
};

///@brief an arbitrary binary tree
class bt{
protected:
	btnode *proot;
	virtual void insert(int)=0;
public:
	bt():proot(NULL){}
	btnode *getroot(){return proot;}
};

///@brief A complete binary tree is a binary tree in which
///every level, except possibly the last, is completely filled, 
///and all nodes are as far left as possible.
///is-a binary tree, but implemented in terms of vector
class bt_complete:public bt{
protected:
	vector<int> v;
	virtual void insert(int){}
public:
	inline int PARENTPOS(int i){return (i/2);}
	inline int LCHILDPOS(int i){return (i*2+1);}
	inline int RCHILDPOS(int i){return (i*2+2);}

	bt_complete(){}
	~bt_complete(){}
};
///@brief A full binary tree (sometimes proper binary tree or 
///2-tree or strictly binary tree) is a tree in which every
///node other than the leaves has two children.
///is-a binary tree, but implemented in terms of vector
class bt_full:public bt{
	vector<int> v;
public:
};

///@brief an binary search tree
///BST is-a Binary Tree
class bst:public bt{
	vector<int> walk_from_node(btnode* p, WALKORDER wo) const;
public:
	bst();
	bst(int* p,int* q);
	bst(int* p,int size);
	bst(const vector<int>& v);

	~bst();

	void insert(int n);
	void insert(int* p,int* q);
	void insert(int* p,int size);
	void insert(const vector<int>& v);

	vector<int> walk(WALKORDER wo, bool norecursive=true) const;
	void nullify();// set all the keys to NULL Do it iteratively in O(N) time with O(1) space complexity where N is the number of nodes in the tree.


	size_t height();
	size_t height_min();
	size_t height_max();
	size_t inbalance();

	vector<int> getdepth(int n);

	static bool isBST(const bst& t);
	static WALKORDER getOrder(const vector<int>& v);
	static bool test();

};

class treeiterator{
public:
	virtual btnode* next()=0;
	virtual btnode* begin()=0;
	virtual btnode* end()=0;
	virtual int operator*()=0;
	/*virtual int operator++()=0;    /// ++()
	virtual int operator++(int)=0; /// ()++
	virtual int operator--()=0;
	virtual int operator--(int)=0;*/
};

/**
bst t;
bst_iterator_postorder bip(t);
while(btnode* p=bip.next()){
	cout << p->d << endl;
}
*/
class bst_iterator_postorder:public treeiterator{
	bst& t;
	stack<btnode*> stk;//ditto
	btnode*tmp;
	set<btnode*> processed_nodes;
	bool pushmode;

	void findfirstnode(){
		tmp=t.getroot();
		while(true){
			stk.push(tmp);
			processed_nodes.insert(tmp);
			if (tmp->l){
				tmp=tmp->l;
			}else if (tmp->r){
				tmp=tmp->r;
			}else{
				stk.pop();
				pushmode=false;
				break;
			}
		}
	}
public:
	bst_iterator_postorder(bst& lhs):t(lhs){
		tmp=NULL;
		pushmode=true;
		findfirstnode();
	}
	~bst_iterator_postorder(){}

	void reset(){
		//tmp=t.getroot();
		pushmode=true;
		processed_nodes.clear();
		findfirstnode();
		//assert(stk.empty());
	}
	virtual btnode* next(){
		// the key point is
		//(1)what is inside the stack  left->left->left
		//(2)when to print the node[push/pop] 
		// the last one must be the root
		//static set<btnode*> processed_nodes; // processed but not printed
		//btnode*tmp=t.getroot();//ditto
		//bool pushmode=true;
		if(stk.empty()){return NULL;}
		tmp=stk.top();
		while(tmp){
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
					return tmp;
				}
			}else{
				if (tmp->r && processed_nodes.count(tmp->r)==0){
					tmp=tmp->r;
					pushmode=true;
				}else{
					stk.pop();
					return tmp;
				}
			}
		}
		reset();
		return NULL;
	}
	virtual btnode* begin(){return tmp;}
	virtual btnode* end(){return t.getroot();}
	virtual int operator*(){return 0;}
};

///@brief read black tree is a binary search tree
class RBbst:public bst{};

///@brief heap is implemented in terms of a binary tree - private inheritance/composition+public inheritance
class heap{
	class heaper:public bt_complete{
	public:
		void insert(int);
		int pop();
		void print();
	} _h;

public:
	void insert(int n){_h.insert(n);}
	int pop(){return _h.pop();}
	void print(){_h.print();}

	static bool test();
};

//////////////////////////////////////////////////////////////////////////
namespace sandbox{
	///@brief heap is implemented in terms of a binary tree - composition
	template<class T>
	class heap{
		bt_complete btc;
	public:
		void insert(const T&);
	};

}




#endif