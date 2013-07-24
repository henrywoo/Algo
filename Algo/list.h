#ifndef __ALGO_DLIST__
#define __ALGO_DLIST__

#include <vector>
using namespace std;

/**@remarks
*Types of linked lists.
*1. Linear linked lists
*2. Circular linked lists.
*3. Double linked lists.
*4. Circular double linked lists.
*5. others: xorlist
*/


struct dlist_node{
	int d;
	dlist_node* next;
	dlist_node* prev;
	dlist_node(int n):d(n){}
};

///@brief A HEAD_TAIL Double Linked List
class dlist{
	dlist_node* psentinel;// no need for front end

	size_t lsize;

	void initialize();
public:

	dlist();

	dlist(int* p,int* q);
	dlist(int* p,int size);
	dlist(const vector<int>& v);

	~dlist();

	/// @brief pass the end, which is the same as the sentinel in this case
	dlist_node* end();
	dlist_node* begin();

	size_t size();

	///@brief push new node as the next of sentinel
	void push_front(int n);
	void push_front(int* p,int* q);


	/// @brief push new node as the previous of sentinel\n
	/// A.next=B then B.prev=A, they always appear as a pair
	void push_end(int n);

	/// @brief swap pointer prev and next for every node including sentinel
	void reverse();
	static void test();
};

///@brief A Circular Double Linked List
class dclist{
	dlist_node* psentinel;// no need for front end

	size_t lsize;

	void initialize();
public:

	dclist();

	dclist(int* p,int* q);
	dclist(int* p,int size);
	dclist(const vector<int>& v);

	~dclist();

	/// @brief pass the end, which is the same as the sentinel in this case
	dlist_node* end();
	dlist_node* begin();

	size_t size();

	///@brief push new node as the next of sentinel
	void push_front(int n);
	void push_front(int* p,int* q);


	/// @brief push new node as the previous of sentinel\n
	/// A.next=B then B.prev=A, they always appear as a pair
	void push_end(int n);

	/// @brief swap pointer prev and next for every node including sentinel
	void reverse();
	static bool test();
};

struct slist_node{
	int d;
	slist_node* next;
	slist_node(int n):d(n),next(NULL){}
};

///@brief A Single Liked List with a head only
class slist
{
public:
	slist();
	slist(int* p,int* q);
	slist(int* p,int size);
	slist(const vector<int>& v);
	~slist();

	///@brief push new node as the next of sentinel
	void push(int n);
	void push(int* p,int* q);

	static bool test();

	slist_node* end();

	///@deprecated no tail for slist
	slist_node* begin();

	///@brief find the kth node before the tail
	///@param k - the kth node before the tail, should be positive
	///@return if k is negative, return NULL;\n
	///        if k+1 is greater than the length of the list, return NULL;\n
	///        if exception occurs, for instance, corrupted list node, return NULL\n
	///        otherwise return a pointer of the kth node before the tail node\n
	///\note The 0th node is the tail itself\n
	///\exception N.A.
	///@author Simon (simonwoo2000@gmail.com)
	///\date 25 July, 2013
	///@test k=0,-1,1,1000000
	///@version 1.0
	///@remarks Two pointer tick, like a sliding window
	///Time - O(n) where n is the length of the list; Space - O(1)
	const slist_node* FindKthToTail(int k) const throw(){
		try{
			if(k<0){
				return NULL;
			}else if(k==0){// just return the tailnode itself
				slist_node* tmp=head;
				slist_node* prevtmp=NULL;// the prev of tmp
				while (tmp!=NULL){
					prevtmp=tmp;
					tmp=tmp->next;
				}
				return prevtmp;
			}else{
				slist_node* tmp1=head;
				slist_node* tmp2=head;
				unsigned int count=0;// the size of the slist
				while(tmp1!=NULL){
					tmp1=tmp1->next;
					if (count++ >k){
						tmp2=tmp2->next;
					}
				}
				if (count < k+1){// no move for tmp2, which means K+1 > length of the link
					return NULL;
				}else{
					return tmp2;
				}
			}
		}catch(...){return NULL;}
	}

	///@brief if there is a circle inside the single list
	bool HasCircle() const {
	}



private:
	slist_node* head;
	size_t lsize;

	
};

///@brief A Circular Single Liked List with a head only
class sclist
{
public:
	sclist();
	sclist(int* p,int* q);
	sclist(int* p,int size);
	sclist(const vector<int>& v);
	~sclist();

	///@brief push new node as the next of sentinel
	void push_front(int n);
	void push_front(int* p,int* q);

private:
	slist_node* sentinel;
	size_t lsize;
};


#endif