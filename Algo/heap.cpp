#include "heap.h"
#include <iostream>
#include <iterator>
#include "util.h"
#include <functional>
#include "sort.h"

using namespace heaping;
using namespace std;

//heap h;

void heap::print(){
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));cout << endl;
}

///@brief bubble up method
///@image html heap-operations.jpg
void heap::insert(int n){
    v.push_back(n);
    int pos=v.size()-1;
    while (PARENTPOS(pos)>=0 && comp(v.at(PARENTPOS(pos)),n))
    {
        util::swap(v.at(PARENTPOS(pos)),v.at(pos));
        if (pos==0){
            break;
        }
        pos=PARENTPOS(pos);
    }
}

void heap::erase(int n){
    int i=search(n);
    v[i]=v.back();
    v.pop_back();
    heapify(i);
}

///@search in a heap
///@note O(n)
///@return the index in the underlying array; -1 if failed to find
///@todo add some auxilary array to store mymin and mymax for every layer; if biggest < 2*15
int heap::search(int n){
    vector<int>::iterator i=v.begin();
    int index=0;
    while (i!=v.end()){
        if (n==*i){
            return index;
        }
        i++;
        index++;
    }
    return -1;
}

///@brief pop one element(mymax/mymin) from heap
///NOTE: vector<T>::pop_back is to remove the last item from the vector
int heap::pop(){
    int r=v.at(0);
    v.at(0)=v.back();
    v.pop_back();
    heapify(0);
    return r;
}

///@brief heapify an array from index pos(inclusive)
///pos 就是vector的index
void heap::heapify(int pos){
    int lastpos=v.size()-1;
    while (pos<lastpos){
        int l=LCHILDPOS(pos);
        int r=RCHILDPOS(pos);
        if (l > lastpos && r > lastpos)
          break;
        if (r > lastpos /*&& l <= lastpos*/){ // coz assert(l<r)
            if (v[pos]<v[l]){
                util::swap(v[pos],v[l]);
                pos=l;
            }
        }else{
          //和子节点里面比较小的交换!!因为是minheap
#if 1
          pair<int&, int> pr = v[l] < v[r] ? 
            pair<int&, int>({ v[l], l }) : 
            pair<int&, int>({ v[r], r });
          if(v[pos]<pr.first)break;
          swap(pr.first, v[pos]);
          pos = pr.second;
#else
          if (v.at(l)>v.at(r)){
            if (v[pos] < v[l]){
              util::swap(v.at(pos), v.at(l));
              pos = l;
            }
          }else{
            if (v[pos] < v[r]){
              util::swap(v.at(pos), v.at(r));
              pos = r;
            }
          }
#endif
        }
    }
}

void heap::update(int* p,int n){
    int pos=p-head();
    *p=n;
    heapify(pos);
}

void heap::update(int pos,int n){
    int *p=head();
    *(p+pos)=n;
    heapify(pos);
}

//bool heap::isheap(int* head,int *tail, bool(*cmp)(int,int)){
bool heap::isheap(int* head,int *tail){
    int pos=0;
    int sz=tail-head+1;
    int lastpos=sz-1;
    int* tmp=head;
    while (tmp <= tail){
        int l=LCHILDPOS(pos);
        if (l<=lastpos){
            if (*tmp >= *(head+l)){
                //
            }else{
                return false;
            }
            int r=RCHILDPOS(pos);
            if (r<=lastpos){
                if (*tmp >= *(head+r)){
                    //
                }else{
                    return false;
                }
            }
        }else{
            break;///NOTE: no need to check if we see a leaf node!!!
        }
        tmp++;
        pos++;
    }
    return true;
}

void heap::print2(){}
void heap::print2()const{}
int* heap::head(){return &*v.begin();}
int* heap::tail(){return &this->v.back();}

bool heap::test(){
    heap h;
    h.print2();
    const heap h2;
    h2.print2();
    int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
    int* b=a;
    while(b!=a+sizeof(a)/sizeof(int)){
        h.insert(*b);
        b++;
    }
    h.print();
    int p=h.pop();
    cout << p << endl;
    h.print();
    h.erase(13);
    h.print();
    cout << boolalpha << heap::isheap(h.head(),h.tail()) << endl;
    h.update(4,1);
    h.print();
    cout << boolalpha << heap::isheap(h.head(),h.tail()) << endl;
    //h.update()

    return true;
}

///@brief maxheap include smaller part; minheap include bigger part
void MedianConstantAccess::insert(int i){
    if (minh.empty() && minh.empty()){
        minh.push(i);
    }else if (minh.size()>maxh.size()){
        int tmp= minh.top();
        if (tmp<i){
            maxh.push(tmp);
            minh.pop();
            minh.push(i);
        }else{
            maxh.push(i);
        }
    }else{
        int tmp= maxh.top();
        if (tmp>i){
            minh.push(tmp);
            maxh.pop();
            maxh.push(i);
        }else{
            minh.push(i);
        }
    }
}

double MedianConstantAccess::get(){
    if (minh.size()==maxh.size()){
        int tmp1=minh.top();
        int tmp2=maxh.top();
        return (tmp1+tmp2)/2.0;
    }else{
        return static_cast<double>(minh.top());
    }
}

bool heaping::test(){
    // Proof that by default, the priority_queue is on a strict weak ordering!
    //typedef priority_queue<int> MaxIntHeap;
    //typedef priority_queue<int,vector<int>,greater<int>> MinIntHeap;
    int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
    MaxHeapInt pq1(a,a+sizeof(a)/sizeof(a[0]));// cannot iterate forward/reverse
    MinHeapInt pq2(a,a+sizeof(a)/sizeof(a[0]));// 

    int tmp1=pq1.top();
    int tmp2=pq2.top();
    cout<< (tmp1==50) << "\t" << (tmp2==5) << endl;

    // Set - RB tree from small to big
    set<int> st(a,a+sizeof(a)/sizeof(a[0]));
    cout << *st.begin() << "\t" << *--st.end() << endl;

    MedianConstantAccess mca;
    //for_each(a,a+sizeof(a)/sizeof(a[0]),ptr_fun(mca.insert));
    for (int i=0;i<=sizeof(a)/sizeof(a[0])-1;i++){
        mca.insert(a[i]);
    }
    //mca.insert()
    cout << mca.get() << endl;

    sorting::quicksort(a,a+sizeof(a)/sizeof(a[0])-1);
    copy(a,a+sizeof(a)/sizeof(a[0]),ostream_iterator<int>(cout,","));

    return true;
}