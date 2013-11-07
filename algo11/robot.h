#include <utility>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <time.h>
#include <fstream>
#include <set>
#include <unordered_map>
#include <string>

#include <windows.h>
#include <bitset>

using namespace std;
typedef unsigned int uint;
#define MAXLINE 51*1000*1000
#define COUNTINGSORTMAXNUM 1<<30

typedef struct bitarray{
  bitarray(uint sz){
    if (sz>0){
      p = new char[sz / 8 + 1];
    }
  }
  ~bitarray(){
    if(p) delete [] p;
  }
  char* p;
  bool get(uint i){
    uint x = i / sizeof(char);
    uint y = i % sizeof(char);
    return p[x] & (1 << y);
  }
  bool set(uint i){
    uint x = i / sizeof(char);
    uint y = i % sizeof(char);
    if (p[x] & (1 << y)){
      p[x] |= (1 << y);
    }
  }
} barray;

// can be optimized for integer less than 1073741824(2**30)
// return size of the bucket
// 1,3,5,10,9,1
int AdaptiveSort(uint* head, uint* tail, uint mi, uint ma) {
  uint hsize = tail - head + 1;
  uint maximum = ma - mi-1;
  barray ba(maximum);
  for (uint i = 0; i < hsize; ++i){
    if (head[i] - mi > 0){
      ba.set(head[i] - mi-1);
    }
  }
  uint j = 0;
  head[j++] = mi;
  for (uint i = 0; i < maximum; ++i){
    if (ba.get(i)){
      head[j++] = i + mi+1;
    }
  }
  return j;
}

struct timer{
  int start;
  const char* str;
  timer(const char* s = "") :start(clock()), str(s){}
  ~timer(){ printf("[%s] Time Cost: %.3lf\n", str, double(clock() - start) / CLOCKS_PER_SEC); }
};

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
    if (!ptree)delete[] ptree;
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


  ///@return -1 means "not in any bucket"
  uint _GetBktNoInSortedArray(uint* head,uint* tail,uint target, uint base=0){
    if (target < *head || target>*tail){ return -1; }//edge case

    int num = 0;
    uint* headbackup = head;
    uint* mid = head + ((tail - head + 1) >> 1);
    while (head<tail){
      if (tail - head == *tail - *head){
        num = target - *head + 1;
        break;
      }else{
        if (head + 1 == tail && *head<target && *tail>target){
          num = mid - headbackup + 1;
          break;
        }
        mid = head + ((tail - head + 1) >> 1);
        if (target==*mid){
          num = mid - headbackup + 1;
          break;
        }else if (target>*mid){
          head = mid;
        }else{
          tail = mid;
        }
      }
    }
    return num + base;
  }

  void BuildBIT(){
    timer t(__FUNCSIG__);
    ptree = new int[bktsz]();
    printf("%d\n",bkt[bktsz-1]);
    //uint hi = bktsz - 1;
#if 0
    if (bkt.back() - bkt.front()+1 == bktsz){
      for (uint i = 0; i < rdata.size() - 1; i = i + 2){
        if (rdata[i] == rdata[i + 1]){
          ++umi[rdata[i]];
        }else{
          uint n1 = rdata[i] - bkt.front() +1;
          uint n2 = rdata[i+1] - bkt.front() + 1;
          __update(n1, n2); // 3s
        }
      }
      vector<uint>().swap(rdata);
    }else
#endif
    {
      for (uint i = 0; i < rdatasz-1; i += 2){
        if (rdata[i] == rdata[i + 1]){
          ++umi[rdata[i]];
        }else{
          uint n1 = __getbucketno(rdata[i], hi);
          uint n2 = __getbucketno(rdata[i + 1], hi, n1 - 1);
          __update(n1, n2);
          //__debug();
        }
      }
    }
    vector<uint>().swap(rdata);
    
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
    bool isboundrypoint = __getbucketno(target, idx, bktsz - 1, 0);
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
