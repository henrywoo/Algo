#ifndef __ALGO_BT__
#define __ALGO_BT__

#include <vector>
#include <stack>
#include <set>
#include <numeric>//accumulate
#include <iostream>

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

///@brief a binary tree
class bt{
private:
    
    int pushleftchildren(btnode* p,stack<btnode*>& rstk, int nbase){
        int r=0;
        while (p){
            rstk.push(p);
            nbase *= 10;
            r=r*10+p->d;
            p=p->l;
        }
        return r+nbase;
    }

public:
    btnode *proot;
    bt():proot(NULL){}
    btnode *getroot(){return proot;}
    ///@brief todo
    ///@link http://leetcode.com/onlinejudge#question_129
    int Sum_Root2Leaf_Numbers(){
        if (proot==NULL){
            return 0;
        }
        stack<btnode*> stk;
        vector<int> v;
        int r=0;//result to be returned
        r=pushleftchildren(proot,stk,r);
        if (stk.top()!=NULL && stk.top()->r==NULL){//leaf node
            v.push_back(r);
        }
        
        //DFS
        btnode* dummy=NULL;
        while(!stk.empty()){
            btnode* bn=stk.top();
            if (bn==dummy){
                stk.pop();
                stk.pop();
                r/=10;
                continue;
            }else if(bn->r){
                stk.push(dummy);///
                r=pushleftchildren(bn->r,stk,r);
                if (stk.top()!=NULL && stk.top()->r==NULL){//leaf node
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
    /*we can declare a global member variable ‘maxValue’ to store the possible 
    max sum value and recursively to compute the max single path sum of and max 
    subtree path sum. The final max sum value can be max(root.value, root.val + 
    leftSubtreeMaxSum, root.val + rightSubTreeMaxSum, root.val + leftSubtreeMaxSum +
    rightSubTreeMaxSum, maxValue).*/
    int maxPathSum(){
        return maxPathSum(proot);
    }

    int maxPathSum(btnode *root) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int csum;
        int maxsum = INT_MIN;
        maxPathSumHelper(root, csum, maxsum);
        return maxsum;
    }

#define max3(a,b,c) (max(a,max(b,c)))
#define max4(a,b,c,d) (max(max(a,b),max(b,c)))
    void maxPathSumHelper(btnode *node, int &csum, int &maxsum) {
        if (!node) {
            csum = 0;
            return;
        }
        printf("%d\n",node->d);
        int lsum = 0, rsum = 0;
        maxPathSumHelper(node->l, lsum, maxsum);
        maxPathSumHelper(node->r, rsum, maxsum);
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
        tmp->l=new btnode(600);
        tmp->r=new btnode(700);

        int s=mybt.Sum_Root2Leaf_Numbers();
        cout << s << endl;

        int mps=mybt.maxPathSum();
        cout << mps << endl;
        return true;
    }


};

inline int PARENTPOS(int i){return (((i+1)>>1)-1);}
inline int LCHILDPOS(int i){return ((i<<1)+1);}
inline int RCHILDPOS(int i){return ((i<<1)+2);}

#endif