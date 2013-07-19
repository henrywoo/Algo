#ifndef __ALGO_SORT__
#define __ALGO_SORT__
#include "util.h"

namespace sorting{
	int c=0;
	void selectionsort(int *p,int *q){
		int sz=q-p;
		int *tmp=p;
		for (int i=0;i<=sz;i++){
			//copy(p,q,ostream_iterator<int>(cout," "));cout << endl;
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

	void test(){

		int c[1]={};

		int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
		selectionsort(a,a+sizeof(a)/sizeof(int)-1);

		int b[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
		insertionsort(b,0,sizeof(b)/sizeof(int));
	}
}



#endif