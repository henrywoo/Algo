#ifndef __ALGO_BST__
#define __ALGO_BST__

#include <vector>

using namespace std;


///@brief arbitrary binary tree node[bst,heap,etc]
struct btnode{
	btnode* l;// left node
	btnode* r;// right node
	int d;
	short color;
	btnode(int n):l(NULL),r(NULL),d(n),color(0){}
};

///@brief an arbitrary binary tree
class bt{
protected:
	btnode *proot;
public:
	bt():proot(NULL){}
};

enum WALKORDER{
		PREORDER=1,
		INORDER,
		POSTORDER,
		LAYERBYLAYER, //layer by layer
		ZIGZAG,
		NONE=-1 //
	};

///@brief an binary search tree
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

	static bool isBST(const bst& t);
	static WALKORDER getOrder(const vector<int>& v);
	static bool test();

};







#endif