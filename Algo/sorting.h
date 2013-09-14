#ifndef __ALGO_SORTING__
#define __ALGO_SORTING__

namespace sorting2{
    
    //qsort说到底是一个递归函数
    template<typename T>
    void qsort(T* head, T* tail){
        if (head==tail || head==NULL  ||tail==NULL){
            return;
        }
        T* h=head;
        T* t=tail-1;// test h=t
        while(h!=t){
            if (*h > *tail){
                while(h!=t){
                    if (*t < *tail){
                        //swap(h,t);
                        
                        T tmp=*h;
                        *h=*t;
                        *t=tmp;

                        t--;
                        break;
                    }
                    t--;
                }
                if (h==t){
                    break;
                }
            }
            h++;
        }
        if (*h>*tail){
            T tmp=*h;
            *h=*tail;
            *tail=tmp;
        }
        T* mid=h;
        h=head;
        qsort<T>(head,mid);
        qsort<T>(mid+1,tail);
    }


    ///@todo
    template<class T>
    void mergesort(T* head,T* tail){}

    ///@todo
    // mergesort multi-array into a vector
    template<class T>
    vector<T> sortmultiplearray(vector<vector<T>>& vvt){}

    void test(){
        int b[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
        qsort<int>(b,b+_countof(b)-1);
    }

}

#endif