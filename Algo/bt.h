#ifndef __ALGO_BT__
#define __ALGO_BT__

#include <vector>
#include <stack>
#include <set>
#include <numeric>//accumulate
#include <iostream>
#include <assert.h>

using namespace std;
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
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

///@brief a binary tree
class bt{
private:
public:
    btnode *proot;
    bt():proot(NULL){}
    btnode *getroot(){return proot;}

    ///@brief todo
    ///@link http://leetcode.com/onlinejudge#question_129
    /*
    Given a binary tree containing digits from 0-9 only, each root-to-leaf path could represent a number.
    An example is the root-to-leaf path 1->2->3 which represents the number 123.
    Find the total sum of all root-to-leaf numbers.
    For example,
     1
    / \
   2   3
    The root-to-leaf path 1->2 represents the number 12.
    The root-to-leaf path 1->3 represents the number 13.
    Return the sum = 12 + 13 = 25.
    */
    //典型的DFS
    //需要两个数据结构:
    //一个是stack计算值，一个vector保存所有的root->leaf形成的数
    //技巧: dummy加入stack表示之后是一个右节点
    //(只对binary tree有效,如果是树就不行了)
     int Sum_Root2Leaf_Numbers() const{
        if (proot==nullptr){
            return 0;
        }
        stack<btnode*> stk;
        vector<int> v;

        int r=0;//global
        auto pushleftchildren = [](btnode* p, stack<btnode*>& rstk, int nbase){
          int r = 0;
          while (p){
            rstk.push(p);
            nbase *= 10;
            r = r * 10 + p->d;
            p = p->l;
          }
          return r + nbase;
        };
        r=pushleftchildren(proot,stk,r);
        assert(!stk.empty());
        auto isleaf = [stk]()->bool{
          return (stk.top() && !stk.top()->l && !stk.top()->r);
        };
        if (isleaf()){//leaf node
            v.push_back(r);
        }
        
        //DFS - stack
        btnode* const dummy=NULL;
        while(!stk.empty()){
            btnode* bn=stk.top();
            if (bn==dummy){
                stk.pop();
                stk.pop();
                r/=10; // 10分位的移动,二进制的移动是>>和<<
                continue;
            }else if(bn->r){
                stk.push(dummy);///
                r=pushleftchildren(bn->r,stk,r);
                if (isleaf()){//leaf node
                  v.push_back(r);
                }
            }else{
                stk.pop();
                r/=10;
            }
        }
        r=accumulate(v.begin(),v.end(),0);
        return r;
    }

    ///@link http://discuss.leetcode.com/questions/288/binary-tree-maximum-path-sum
/*
Given a binary tree, find the maximum path sum. 
The path may start and end at ""any"" node in the tree. 
For example:
Given the below binary tree, 
       1
      / \
     2   3

Return 6.

we can declare a global member variable ‘maxValue’ to store the possible 
max sum value and recursively to compute the "max single path sum" and "max 
subtree path sum". The final max sum value can be:
    
    max(root.value, 
    root.val + leftSubtreeMaxSum,
    root.val + rightSubTreeMaxSum,
    root.val + leftSubtreeMaxSum +rightSubTreeMaxSum,
    maxValue).
*/
    int maxPathSum(){
        return maxPathSum(proot);
    }

    int maxPathSum(btnode *root) {
        int csum;
        int maxsum = INT_MIN;
        maxPathSumHelper(root, csum, maxsum);
        return maxsum;
    }

    void maxPathSumHelper(btnode *node, int &csum, int &maxsum) {
        if (!node) {
            csum = 0;
            return;
        }
        printf("%d\n",node->d);
        int lsum = 0, rsum = 0;
        maxPathSumHelper(node->l, lsum, maxsum);
        maxPathSumHelper(node->r, rsum, maxsum);
        auto max3 = [](int a, int b, int c)->int{
          return (max(a, max(b, c)));
        };
        csum = max3(node->d,
            node->d + lsum,
            node->d + rsum);
        maxsum = max3(maxsum,
            csum,
            node->d + lsum + rsum);
    }

    bt getMirror(){
        bt newbt;
        ///@todo
        return newbt;
    }

    static bool test(){
        //construct a binary tree
        //         1
        //       /   \
        //     2       9
        //   /   \
        // 3      4
        //  \     /
        //  8     5
        //       /\
        //      6  7
        bt mybt;
        mybt.proot = new btnode(1);
        mybt.proot->l = new btnode(2);
        mybt.proot->r = new btnode(9);
        btnode* tmp=mybt.proot->l, *tmp2=NULL;
        tmp->l=new btnode(3);
        tmp->r=new btnode(4);
        tmp2=tmp->r;
        tmp=tmp->l;
        tmp->r=new btnode(8);
        tmp2->l=new btnode(5);
        tmp=tmp2->l;
        tmp->l=new btnode(6);
        tmp->r=new btnode(7);

        int s=mybt.Sum_Root2Leaf_Numbers();
        cout << s << endl;

        // 1->9->4->8->7
        bt mybt2;
        mybt2.proot = new btnode(1);
        mybt2.proot->r = new btnode(9);
        tmp = mybt2.proot->r;
        tmp->r = new btnode(4);
        tmp = tmp->r;
        tmp->r = new btnode(8);
        tmp = tmp->r;
        tmp->r = new btnode(7);
        s = mybt2.Sum_Root2Leaf_Numbers();
        cout << s << endl;

        int mps=mybt2.maxPathSum();
        cout << mps << endl;
        return true;
    }
};

/*
         0
      /     \
     1       2
    / \     /  \
   3    4   5   6
  / \   /
 7   8 9
*/

inline int PARENTPOS(int i){return (i-1) >> 1;/*(((i+1)>>1)-1);*/}
inline int LCHILDPOS(int i){return ((i<<1)+1);}
inline int RCHILDPOS(int i){return ((i+1)<<1);}

#endif