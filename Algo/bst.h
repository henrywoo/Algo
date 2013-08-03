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
	ZIGZAG,// via two stacks
	NONE=-1 //
};

///@brief arbitrary binary tree node[bst,heap,etc]
struct btnode{
	btnode* l;// left node
	btnode* r;// right node
	int d;
	short color;
	btnode* par;
	double weight;
	btnode(int n):l(NULL),r(NULL),d(n),color(0),weight(.0){}
};

///@brief an arbitrary binary tree
///@interface an ABC(abstract base class) with pure virtual function of a binary tree
class bt{
protected:
	btnode *proot;
	virtual void insert(int)=0;
public:
	bt():proot(NULL){}
	btnode *getroot(){return proot;}
};

inline int PARENTPOS(int i){return (((i+1)>>1)-1);}
inline int LCHILDPOS(int i){return ((i<<1)+1);}
inline int RCHILDPOS(int i){return ((i<<1)+2);}
///@brief A complete binary tree is a binary tree in which
///every level, except possibly the last, is completely filled, 
///and all nodes are as far left as possible.
///is-a binary tree, but implemented in terms of vector
class bt_complete:public bt{
protected:
	vector<int> v;
	virtual void insert(int){}
public:
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
    /// implementation
	vector<int> walk_from_node(btnode* p, WALKORDER wo) const;
    bool save2XML_recursive_hlp(btnode*,ofstream& f);
public:
    /// interface
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

    ///set all the keys to NULL Do it iteratively in O(N) time with O(1)
    ///space complexity where N is the number of nodes in the tree.
	void nullify();

	size_t height();
	size_t height_min();
	size_t height_max();
	size_t inbalance();

	vector<int> getdepth(int n);
    size_t diameter();

    
    bool save2XML();// 2 means to
    bool save2Jason();

	static bool isBST(const bst& t);
	static WALKORDER getOrder(const vector<int>& v);
    static bst read4XML(const char*);// 4 means from
    static bool compareBST(const bst&, const bst&);
	static bool test();
private:
    size_t height(btnode*);

    ///http://www.geeksforgeeks.org/diameter-of-a-binary-tree/
    ///@todo: to be optimised
    size_t diameter(btnode*);
};

class treeiterator{
public:
	virtual btnode* next()=0;
	virtual btnode* begin()=0;
	virtual btnode* end()=0;
	virtual int operator*()=0;
	//virtual treeiterator& operator++()=0;    /// ++()
	//virtual treeiterator operator++(int)=0; /// ()++
	//virtual int operator--()=0;
	//virtual int operator--(int)=0;
};

///@brief a postorder Binary Search Tree iterator
class bst_iterator_postorder:public treeiterator{
	bst& t;
	stack<btnode*> stk;//ditto
	btnode*p2curr_node; //pointer to current node
	set<btnode*> processed_nodes;
	bool pushmode;

	void findfirstnode();
public:
	bst_iterator_postorder(bst& lhs);
	~bst_iterator_postorder();

	void reset();

    ///the key point is
    ///(1)what is inside the stack  left->left->left
    ///(2)when to print the node[push/pop] 
    /// the last one must be the root
    ///static set<btnode*> processed_nodes; // processed but not printed
    ///btnode*tmp=t.getroot();//ditto
    ///bool pushmode=true;
	btnode* next();

    ///pre-autoincrement
    bst_iterator_postorder& operator++();

    ///post-autoincrement
    bst_iterator_postorder operator++(int);

	btnode* begin(){return p2curr_node;}
	btnode* end(){return t.getroot();}
	int operator*(){return p2curr_node->d;}
    ///Member functions of the form
    ///   operator TypeName()
    ///are conversion operators. They allow objects of the class type to be
    ///used as if they were of type TypeName and when they are, they are 
    ///converted to TypeName using the conversion function.
    ///http://stackoverflow.com/questions/4600295/what-is-the-meaning-of-operator-bool-const-in-c
    operator bool()const{return p2curr_node!=NULL;}
};

///@brief read black tree is a binary search tree
class RBbst:public bst{};

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