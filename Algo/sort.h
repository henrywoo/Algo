#ifndef __ALGO_SORT__
#define __ALGO_SORT__

#include "util.h"
#include "assert.h"
#include "radixsort.h"
/// quicksort - http://tee-cis55datastructure.blogspot.sg/2011/04/quicksort-practice-in-lab-exercises.html

#define ARRSIZE(a,b) (sizeof(a)/sizeof(b))

namespace sorting{
    #define MAXEDGE 101

	void selectionsort(int *p,int *q);

    bool greater(int a,int b);
    bool lesser(int a,int b);

	void insertionsort(int* head,int* tail, bool(*cmp)(int,int)=greater);

	void insertionsort(int a[],int start=0,int end=32);

	vector<int> merge_sub(int* head1,int* tail1, int* head2, int* tail2);
	void merge_sub(int* head1,int* tail1, int* tail2);
	// head tail != begin end
	void mergesort_bottomup(int* head,int* tail);

	void mergesort_topdown(int* head,int* tail);

	///@brief merge two sorted disjoint spaces from small to big
	vector<int> merge_sub(int* head1,int* tail1, int* head2, int* tail2);
	///@brief merge two sorted consecutive spaces from small to big
	void merge_sub(int* head1,int* tail1, int* tail2);


	int* quicksort_sub_comparision(int* head,int* tail);
	int* quicksort_sub_inplace(int* head,int* tail);
	void quicksort(int* head,int* tail);

	void quicksort2(int* head,int* tail);

	///@brief quick sort by comparison sort and more space, O(n) extra storage space
	int* quicksort_sub_comparision(int* head,int* tail);

	/// @brief used in quick sort and findkthsmallest
	int* quicksort_sub_inplace(int* head,int* tail);

	int findKthsmallest(int* head,int* tail,int k);

	vector<int> TOPOLOGICAL_SORT(int [][MAXEDGE]);

	///@brief A matrix using linear memory space
	///@author WuFuheng(fuhengwu@gmail.com)
	struct matrix;

	

    class TOPOLOGICAL_SORT{
        int INDEG[MAXEDGE];
        int node;
        int edge;
    public:
        vector<int> sort(int S[][MAXEDGE]);
        int m();
    };
	

	void test();
}



#endif