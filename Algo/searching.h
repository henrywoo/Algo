#ifndef __ALGO_SEARCHING__
#define __ALGO_SEARCHING__
#include "sort.h"
#include <iostream>
using namespace std;

namespace searching{

    ///@brief search one int pointer pointing to a int = target
    ///@return the pointer pointing to the int; if not found, return NULL.
    int* binarysearch(int *head, int *tail, int target){
        if (head==NULL || NULL==tail){
            return NULL;
        }else if(head==tail){
            if (*head==target){
                return head;
            }else{
                return NULL;
            }
        }else{
            int* mid=head + (tail-head+1)/2 -1;
            if (target<*mid){
                return binarysearch(head,mid,target);
            }else if(target==*mid){
                return mid;
            }else{
                return binarysearch( mid+1 ,tail,target);///
            }
        }
    }

    int fancybinarysearch(){
        int i=0;
        return i;
    }

    void test(){
        //functional test
        int a[]={1,  3,  -1, -3,  5,  3,  6,  7};
        sorting::insertionsort(a,a+sizeof(a)/sizeof(a[0])-1);
        int* p=binarysearch(a,a+sizeof(a)/sizeof(a[0])-1,1);
        cout << "index: " << p-a << endl;
        //boundary test
        p=binarysearch(a,a,1);
        if (p==NULL){
        }else{
            cout << "index: " << a-p << endl;
        }
        
        p=binarysearch(a,NULL,1);
        cout << "index: " << a-p << endl;


    }

}




#endif