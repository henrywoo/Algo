#ifndef __ALGO_SORT__
#define __ALGO_SORT__
#include "util.h"
#include "assert.h"
/// quicksort - http://tee-cis55datastructure.blogspot.sg/2011/04/quicksort-practice-in-lab-exercises.html

#define ARRSIZE(a,b) (sizeof(a)/sizeof(b))

namespace sorting{

	void selectionsort(int *p,int *q){
		int sz=q-p;
		int *tmp=p;
		for (int i=0;i<=sz;i++){
			copy(p,q,ostream_iterator<int>(cout," "));cout << endl;
			int *ma=tmp;
			int sz2=q-tmp;
			for (int j=0;j<=sz2;j++){
				if (*ma<tmp[j])
					ma=tmp+j;
			}
			util::swap(*ma,*(tmp));
			tmp++;
		}
	}

	void insertionsort(int* head,int* tail){
		int sz=tail-head+1;
		for(int i=1;i<=sz-1;i++){
			for (int j = i-1; j>=0; j--){
				if (head[j]<head[j+1]){
					swap(head[j],head[j+1]);
				}else{break;}
			}
		}
	}

	void insertionsort(int a[],int start=0,int end=32){
		for (int i=start+1;i<end;i++){
			for(int j=i-1;j>=start+0;j--){
				if (a[j+1]>a[j]){
					swap(a[j],a[j+1]);
					cout << "swap " << a[j] << " " << a[j+1] << endl;
				}else{break;}
			}
		}
	}

	vector<int> merge_sub(int* head1,int* tail1, int* head2, int* tail2);
	void merge_sub(int* head1,int* tail1, int* tail2);
	// head tail != begin end
	void mergesort_bottomup(int* head,int* tail){
		size_t sz=tail-head+1;
		int blocksize=1;// range of the inner most subset
		vector<int> v(head,tail+1);

		while (blocksize<=sz/2){
			for (int headindex=0;headindex<=sz-1;headindex=headindex+2*blocksize){
				// 1 - boundary check
				int tmpindex1=headindex;
				int* head1=&v[tmpindex1];

				int tmpindex2=headindex+blocksize-1;
				if (tmpindex2>sz-1){
					tmpindex2=tmpindex1;
				}
				int* tail1=&v[tmpindex2];

				int tmpindex3=headindex+blocksize+1;
				if (tmpindex3>sz-1){
					tmpindex3=tmpindex2;
				}
				int* head2=&v[tmpindex3];

				int tmpindex4=headindex+2*blocksize+1;
				if (tmpindex4>sz-1){
					tmpindex4=tmpindex3;
				}
				int* tail2=&v[tmpindex4];

				merge_sub(head1, tail1, tail2);
			}
			blocksize*=2;
		}
		copy(v.begin(),v.end(),head);
	}

	void mergesort_topdown(int* head,int* tail){
		int sz=tail-head+1;
		if (sz==1){
			return;
		}
		int tmp=sz/2;
		mergesort_topdown(head,head+tmp-1);
		mergesort_topdown(head+tmp,tail);
		merge_sub(head,head+tmp-1,tail);
	}

	///@brief merge two sorted disjoint spaces from small to big
	vector<int> merge_sub(int* head1,int* tail1, int* head2, int* tail2)
	{
		if (head1==head2/* && head1==tail1 && head2==tail2 && tail1==tail2*/){
			return vector<int>(head1,head1+1);
		}
		//assert(head1<tail1 && tail1<head2 && head2<tail2);
		assert(head1<=tail1 && head2<=tail2 && tail1<head2);
		vector<int> v(head1,tail1+1);
		//v.reserve(tail1-head1+1 + tail2-head2+1);
		int sz1=v.size();
		int sz2=tail2-head2+1;
		int sztotal=sz1+sz2;
		v.insert(v.end(),head2,tail2+1);
		///@comment insertion sort (1). from 0 to sz-1 (2). from j to 0
		int* tmp=&v[sz1];
		while(tmp<=&v[sztotal-1]){
			// v[i] - the node before the tmp node
			for (int i=tmp-&v.at(0)-1;i>=0;i--){//int i=tmp-head1
				if (v[i+1]<v[i]){
					util::swap(v[i+1],v[i]);
				}else{
					break;
				}
			}
			tmp++;
			//copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));printf("\n");
		}
		return v;
	}

	///@brief merge two sorted consecutive spaces from small to big
	void merge_sub(int* head1,int* tail1, int* tail2){
		if (/*head1==tail1 && */tail1==tail2){
			return;
		}
		int* head2=tail1+1;
		vector<int>v= merge_sub(head1, tail1, head2, tail2);
		copy(v.begin(),v.end(),head1);
	}


	int* quicksort_sub_comparision(int* head,int* tail);
	int* quicksort_sub_inplace(int* head,int* tail);
	void quicksort(int* head,int* tail){
		if (tail-head+1<=1){return;}
		int* pivotpointer=quicksort_sub_comparision(head,tail);
		quicksort(head,pivotpointer-1);
		quicksort(pivotpointer+1,tail);
	}

	void quicksort2(int* head,int* tail){
		//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
		if (tail-head+1<=1){return;}
		int* pivotpointer=quicksort_sub_inplace(head,tail);
		//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
		quicksort2(head,pivotpointer-1);
		quicksort2(pivotpointer+1,tail);
	}

	///@brief quick sort by comparison sort and more space, O(n) extra storage space
	int* quicksort_sub_comparision(int* head,int* tail){
		int pivot=*tail;
		int sz=tail-head+1;
		vector<int> small, big;
		int* tmp=head;
		while(tmp<tail){
			if (*tmp < pivot){
				small.push_back(*tmp);
			}else{
				big.push_back(*tmp);
			}
			tmp++;
		}
		int smallsize=small.size();
		int bigsize  =big.size();
		copy(small.begin(),small.end(),head);
		head[smallsize]=pivot;
		copy(big.begin(),big.end(),head+smallsize+1);
		return head+smallsize;
	}

	int* quicksort_sub_inplace(int* head,int* tail){
		//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
		int pivot=*tail;

		int* L=head;// the left of L is less than pivot
		int* R=tail-1;// the right of R is greater than pivot
		OUTTER:while (L<R){
			if (*L>pivot){
				//waiting for right node to find a node to swap
				while(L<R){
					if (*R<pivot){
						util::swap(*L,*R);
						//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
						R--;
						goto INNER;
					}
					R--;
				}
				goto OUTTER;// cannot find pair to swap, waiting is ended and R==L right now!
			}
			INNER:
			L++;
		}
		//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
		if(*L>pivot){
			util::swap(*tail,*L);
		}else if(pivot==*L){
		}else{
			L++;
			util::swap(*tail,*L);
		}
		//copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
		return L;
	}

	int findKthsmallest(int* head,int* tail,int k){//smallest -> k=1
		int sz=tail-head+1;
		if (sz<k){return INT_MIN;}
		copy(head,tail+1,ostream_iterator<int>(cout," "));cout<< endl;
		// the node pivot pointing is the (pivot-head+1) small,(tail-pivot+1) big node
		while (true){
			int* pivot=quicksort_sub_inplace(head,tail);
			copy(head,tail+1,ostream_iterator<int>(cout," "));cout<< endl;
			if (k==pivot-head+1){
				return *pivot;
			}else if(k>pivot-head+1){//X=(pivot-head+1) is the Xth smallest one we've found
				k=k-(pivot-head+1);
				head=pivot+1;
			}else{
				//k=(pivot-head+1)-k;////SALT - because here is the kth SMALLEST element
				tail=pivot-1;
			}
		}
	}


	
#include <time.h>
	void test(){
		{
#if 0
            const int N=static_cast<int>(1e3);
			int* a=new int[N];
			for(int i=0;i<N;i++){a[i]=i;}
			random_shuffle(a,a+N);
			time_t start,end;
			time (&start);

			//sort(a,a+N);
			quicksort2(a,a+N-1);
			//mergesort2(a,0,N-1);
			//mergesort(a,0,N-1);
			//sort(a,a+N);
			//stable_sort(a,a+N);

			time (&end);
			double dif = difftime (end,start);
			printf ("It took you %.2lf seconds to sort.\n", dif ); //It took you 31.00 seconds to sort.
			//printarray(a,15200,15250);
			copy(a,a+100,ostream_iterator<int>(cout," "));
			delete [] a;
			//printf("memnum:%d\n",memnum);
#endif
		}
		
		{
			int b[]={5, 1, 3, 6, 0, 4, 7, 2};
			int sz=ARRSIZE(b,int);
			int k=0;
			if (sz&0x1==1){
				k=sz/2+1;
				int median=findKthsmallest(b,b+sizeof(b)/sizeof(int)-1,k);// the smallest one
			}else{
				k=sz/2;
				int median1=findKthsmallest(b,b+sizeof(b)/sizeof(int)-1,k);// the smallest one
				int median2=findKthsmallest(b,b+sizeof(b)/sizeof(int)-1,k+1);// the smallest one
				int median=median1/2+median2/2;
			}
			
			//int b[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
			quicksort2(b,b+sizeof(b)/sizeof(int)-1);
			copy(b,b+ARRSIZE(b,int),ostream_iterator<int>(cout,","));cout << endl;
		}
		{
			int b[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
			quicksort(b,b+sizeof(b)/sizeof(int)-1);
			copy(b,b+ARRSIZE(b,int),ostream_iterator<int>(cout," "));cout << endl;
		}
		{
			int b[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
			insertionsort(b,b+sizeof(b)/sizeof(int)-1);
			copy(b,b+ARRSIZE(b,int),ostream_iterator<int>(cout," "));cout << endl;
		}
		{
			int c[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
			mergesort_topdown(c,c+sizeof(c)/sizeof(int)-1);
			copy(c,c+sizeof(c)/sizeof(int),ostream_iterator<int>(cout," "));cout << endl;
		}
		{
			int c[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
			mergesort_bottomup(c,c+sizeof(c)/sizeof(int)-1);
			copy(c,c+sizeof(c)/sizeof(int),ostream_iterator<int>(cout," "));cout << endl;
		}
		{
			int a1[]={5,7 ,9 ,13 ,19 ,23 ,40, 50};
			int b1[]={6,7 ,8 ,9 ,10 ,19 ,21 ,23 ,25,50};
			vector<int> v=merge_sub(a1,a1+sizeof(a1)/sizeof(int)-1,b1,b1+sizeof(b1)/sizeof(int)-1);
			copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
			cout << endl;
		}
		{
			int a1[]={5,7 ,9 ,13 ,19 ,23 ,40, 50};
			int b1[]={6,7 ,8 ,9 ,10 ,19 ,21 ,23 ,25,50};
			vector<int> v=merge_sub(a1,a1,b1,b1);
			copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
			cout << endl;
		}
	}
}



#endif