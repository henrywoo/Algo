#include <utility>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <time.h>
#include <fstream>
#include <set>
#include <unordered_map>
#include <string>
#include <iostream>

#include <windows.h>
#include <bitset>
#include <functional>

using namespace std;
typedef unsigned int uint;
#define MAXLINE 51*1000*1000
#define COUNTINGSORTMAXNUM 1<<30

struct timer{
  int start;
  const char* str;
  timer(const char* s = "") :start(clock()), str(s){}
  ~timer(){ printf("[%s] Time Cost: %.3lf\n", str, uint(clock() - start) / CLOCKS_PER_SEC); }
};

typedef struct bitarray{
  bitarray(uint sz){
    if (sz>0){
      p = new char[sz / 8 + 1]();
    }
  }
  ~bitarray(){
    if(p) delete [] p;
  }
  char* p;
  bool get(uint i){
    uint x = i / (CHAR_BIT);
    uint y = i % (CHAR_BIT);
    return p[x] & (1 << y);
  }
  void set(uint i){
    uint x = i / (CHAR_BIT);
    uint y = i % (CHAR_BIT);
    if ((p[x] & (1 << y))==0){
      p[x] |= (1 << y);
    }
  }
} barray;

// can be optimized for integer less than 1073741824(2**30)
// return size of the bucket
// 1,3,5,10,9,1

void RadixSort(uint* begin, uint* end) {
  std::vector<uint> bitQueues[2];

  uint bufsz = end - begin;
  uint sz1 =0, sz2 = 0;
  for (size_t bit = 0; bit < CHAR_BIT * sizeof(uint); ++bit) {
    for (uint* itr = begin; itr != end; ++itr) {
      bitQueues[(*itr & (1u << bit)) ? 1 : 0].push_back(*itr);
    }
    if (bit == CHAR_BIT * sizeof(uint)-1)
      bitQueues[0].swap(bitQueues[1]);
    if (sz1 = bitQueues[0].size())
      memcpy_s(begin, bufsz*sizeof(uint), &bitQueues[0][0], sz1*sizeof(uint));
    if (sz2 = bitQueues[1].size())
      memcpy_s(begin + sz1, (bufsz - sz1)*sizeof(uint),&bitQueues[1][0], sz2*sizeof(uint));
    bitQueues[0].clear();
    bitQueues[1].clear();
  }
}

uint getmean(uint *head, size_t sz){
  timer t(__FUNCSIG__);
  uint *end = head + sz;
  uint *tmp = head;
  int i = 1;
  double mean = *head;
  while (++head < end){
    mean = mean * i / (i + 1) + *head / (i + 1.0);
    i++;
  }
  return (uint)mean;
}


uint* mypartition(uint* head, uint* tail, uint pivot){
  //copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
  //int pivot = *tail;

  uint* L = head;// the left of L is less than pivot
  uint* R = tail;// the right of R is greater than pivot
OUTTER:while (L<R){
  if (*L>pivot){
    //waiting for right node to find a node to swap
    while (L < R){
      if (*R < pivot){
        swap(*L, *R);
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
       copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
       if (*L > pivot){
         swap(*tail, *L);
       }
       else if (pivot == *L){
       }
       else{
         L++;
         swap(*tail, *L);
       }
       //copy(head,tail+1,ostream_iterator<int>(cout," "));cout << endl;
       return L;
}

/// radix sort 2.806s
int AdaptiveSort(uint* head, uint* tail, uint mi, uint ma) {
  uint m=getmean(head, tail - head + 1);
  uint hsize = tail - head + 1;
  uint maximum = ma - mi - 1;
  if (maximum > (1 << 20)){
    //mypartition(head, tail, 16);
    partition(head, tail + 1, bind2nd(less<uint>(),m));
    //copy(head, tail + 1, ostream_iterator<int>(cout, " ")); cout << endl;
  }
  
  

  timer t(__FUNCSIG__);
  if (maximum > (1<<20)){//65536
    RadixSort(head, tail+1);
    //sort(head, tail + 1);//0.891
    //copy(head, tail+1, ostream_iterator<int>(cout, " ")); cout << endl;
    return unique(head, tail+1)-head+1;
  }else{
    barray ba(maximum);
    for (uint i = 0; i < hsize; ++i){
      if (head[i] - mi > 0){
        ba.set(head[i] - mi - 1);
      }
    }
    uint j = 0;
    head[j++] = mi;
    for (uint i = 0; i < maximum; ++i){
      if (ba.get(i)){
        head[j++] = i + mi + 1;
      }
    }
    return j;
  }
}


class Robot{
public:
  int* ptree;//int array

  uint* rdata;
  uint rdatasz;
  uint* bkt;
  uint bktsz;

  unordered_map<uint, uint> umi;

  Robot(uint* bkt_, uint bktsz_, uint* rdata_, uint rdatasz_) :
    bkt(bkt_), rdata(rdata_), ptree(NULL){}

  ~Robot(){
    if (!ptree) delete[] ptree;
  }


  void __update(uint first, uint second){
    uint i = first, tmp = 0;
    while (i >= 0 && i <= bktsz){
      tmp = i - 1;
      ++ptree[tmp];
      i += (i & -(int)i);
    }
    i = second;
    while (i >= 0 && i <= bktsz){
      tmp = i - 1;
      --ptree[tmp];
      i += (i & -(int)i);
    }
  }
#if 0

  //binary search O(logN) - todo
  uint __getbucketno(int target, uint hi, uint lo = 0){
    uint mid=0;
    uint tmp = hi;
    while (lo <= hi){
      mid = lo + (hi - lo) / 2;
      if (bkt[mid] == target)
        return mid + 1;
      else if (bkt[mid] < target)
        lo = mid + 1;
      else
        hi = mid - 1;
    }
    return -1;// not found
  }

  void __debug(){
#ifdef _DEBUG
    for (int i = 0; i < bktsz; ++i){
      printf("%d\n", ptree[i]);
    }
    printf( "***********************\n" );
#endif // DEBUG
  }

  ///@return isboundrypoint
  bool __getbucketno(uint target, uint& n, uint hi, uint lo){
    uint mid = 0;
    while (lo < hi){
      if (target > bkt[hi] || target < bkt[lo]){
        n = UINT_MAX;
        return false;
      }else if (lo + 1 == hi && bkt[lo] < target && target < bkt[hi]){
        n = lo + 1;
        return false;
      }
      mid = lo + (hi - lo) / 2;
      if (bkt[mid] == target){
        n = mid + 1;
        return true;
      }else if (bkt[hi] == target){
        n = hi + 1;
        return true;
      }
      else if (bkt[mid] < target)
        lo = mid;
      else
        hi = mid;
    }
    throw 1;
  }
#endif

  ///@return num == UINT_MAX means "not in any bucket"
  ///@return isboundrypoint
  bool _GetBktNoInSortedArray(uint* head,
    uint* tail,
    uint  target,
    uint& num,
    bool fuzzy=false)
  {
    if (target < *head || target>*tail){
      num = UINT_MAX;
      return false;//edge case
    }else{
      uint* headbackup = head;
      uint* mid = 0;
      while (head < tail){
        if (tail - head == *tail - *head){
          num += target - *head + 1;
          return true;
        }else{
          if (fuzzy && head + 1 == tail && *head<target && *tail>target){
            num += head - headbackup + 1;
            return false;
          }
          mid = head + ((tail - head + 1) >> 1);
          if (target == *mid){
            num += mid - headbackup + 1;
            return true;
          }else if (target > *mid){
            head = mid;
          }else{
            tail = mid;
          }
        }
      }
    }
    throw 1;// exception happens
  }

  void BuildBIT(){
    timer t(__FUNCSIG__);
    ptree = new int[bktsz]();
    //printf("%d\n",bkt[bktsz-1]);
    for (uint i = 0; i < rdatasz - 1; i += 2){
      if (rdata[i] == rdata[i + 1]){
        ++umi[rdata[i]];
      }else{
        uint n1=0;
        _GetBktNoInSortedArray(bkt, bkt + bktsz - 1, rdata[i],n1);
        uint n2=n1;
        _GetBktNoInSortedArray(bkt + n1 - 1, bkt + bktsz - 1, rdata[i + 1],n2);
        __update(n1, n2);
        //__debug();
      }
    }
  }

  uint __query(uint idx){
    if (idx==UINT_MAX){
      return 0;
    }
    int sum = 0, tmp = 0;
    while (idx > 0){
      tmp = idx - 1;
      sum += ptree[tmp];
      idx -= (idx&-(int)idx);
    }
    
    return sum;
  }

  uint query(uint target){
    uint idx=0;
    bool isboundrypoint = _GetBktNoInSortedArray(bkt, bkt + bktsz - 1,target, idx, true);
    uint m1 = __query(idx);
    if (isboundrypoint){
      uint m2 = 0;
      if(idx > 1)
        m2=__query(idx - 1);
      int r = m2 > m1 ? m2 : m1;
      if (umi.find(target) != umi.end()){
        r += umi[target];
      }
      return r;
    }else{
      return m1;
    }
    
  }


};
