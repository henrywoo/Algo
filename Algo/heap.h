#ifndef __ALGO_HEAP__
#define __ALGO_HEAP__

#include "bst.h"
#include <iostream>
#include <functional>
#include <vector>
#include <queue>

namespace heaping{
    using namespace std;

    typedef priority_queue<int> MaxIntHeap;///typedef bool(*COMP)(int,int);
    typedef priority_queue<int,vector<int>,greater<int>> MinIntHeap;

    class MedianConstantAccess{
        MaxIntHeap maxh;
        MinIntHeap minh;
    public:
        void insert(int);
        double get();
    };

    ///@brief heap is implemented in terms of a binary tree
    ///private inheritance/composition+public inheritance
    ///which means: heap is NOT a binary tree, but heap can
    ///be implemented as a binary tree
    ///default mymax heap
    typedef bool(*COMP)(int,int);
    class heap: public bt_complete
    {
    private:
        static bool greater(int x,int y){return x>y;}
        static bool lessorequal(int x,int y){return x<=y;}
        COMP comp;
        int minvec[10];///mymin node of every layer!!
        void heapify(int pos);
    public:

        heap(COMP p=heap::greater):comp(p){
            using namespace std;
            cout<< __FILE__ << __LINE__ << __FUNCDNAME__ << endl;
        }
        void insert(int n);
        int pop();
        void print();
        void print2();
        void print2()const;

        void erase(int pos);
        int* head();
        int* tail();
        int search(int i);
        void update(int* p,int n);
        void update(int pos,int n);

        static bool test();
        static bool isheap(int* head,int* tail);
        friend heap mergeheap(const heap& h1,const heap& h2){
            heap h;
            return h;
        }

    };

    class minmaxheap{};

    class deap{};

    ///@brief https://en.wikipedia.org/wiki/Binomial_heap
    class binomialheap{};

    class fibonacciheap{};

    ///http://stackoverflow.com/questions/10812769/static-function-declared-but-not-defined-in-c
    //static bool test();
    bool test();
}



#endif