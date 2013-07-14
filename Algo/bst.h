#ifndef __ALGO_BST__
#define __ALGO_BST__

#include <vector>

using namespace std;

struct bstnode{
	bstnode* l;// left node
	bstnode* r;// right node
	int d;
	short color;
	bstnode(int n):l(NULL),r(NULL),d(n),color(0){}
};

/*struct bstnode2: public bstnode{
	bstnode2* p; // parent
}*/


enum WALKORDER{
		PREORDER=1,
		INORDER,
		POSTORDER,
		LAYERBYLAYER, //layer by layer
		NONE=-1 //
	};
//template <>
class bst{
	bstnode* proot;

	vector<int> walk_from_node(bstnode* p, WALKORDER wo, bool recursive);
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

	vector<int> walk(WALKORDER wo, bool recursive);


	size_t height();
	size_t height_min();
	size_t height_max();
	size_t inbalance();

};

bool isBST(const bst& t);
WALKORDER getOrder(const vector<int>& v);




#endif