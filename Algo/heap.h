#ifndef __ALGO_HEAP__
#define __ALGO_HEAP__

#include "bst.h"

namespace heaping{

    ///@brief heap is implemented in terms of a binary tree
    ///private inheritance/composition+public inheritance
    ///which means: heap is NOT a binary tree, but heap can be implemented as a binary tree
    ///default max heap
    typedef bool(*COMP)(int,int);
    class heap: public bt_complete
    {
    private:
        static bool greater(int x,int y){return x>y;}
        static bool lessorequal(int x,int y){return x<=y;}
        COMP comp;
        int minvec[10];///min node of every layer!!
        void heapify(int pos);
    public:

        heap(COMP p=heap::greater):comp(p){}
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
    class binomialheap{

    };

    class fibonacciheap{};
}



#endif