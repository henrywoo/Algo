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
#include <chrono>

using namespace std;
typedef unsigned int uint;
#define MAXLINE 51*1000*1000
#define COUNTINGSORTMAXNUM 1<<30

using namespace chrono;
struct timer{
  time_point<system_clock> start;
  const char* str;
  timer(const char* s = "") :start(chrono::high_resolution_clock::now()), str(s){}
  ~timer(){
    //printf("[%s] Time Cost: %.3lf\n", str, uint(clock() - start) / CLOCKS_PER_SEC);
    cout << str << ": " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count()
    //cout << str << ": " << duration_cast<nanoseconds>(high_resolution_clock::now() - start).count()
      << " milliseconds\n" << endl;
  }
};
void myswap(uint& a, uint& b){
  if (a != b){
    a = a^b;
    b = a^b;
    a = a^b;
  }
}
void printBit(uint i){
  for (uint count = 0; count < 32; count++){
    uint j = i & 1;
    printf("%d", j);
    i >>= 1;
  }
  printf("\n");
}

typedef struct bitarray{
  explicit bitarray(uint range):p(NULL),
  blocknum(range / (sizeof(uint)*CHAR_BIT) + 1),
  bitnum(blocknum*(sizeof(uint)*CHAR_BIT))
  {
    if (range>0){
      p = new uint[blocknum]();
      
    }
  }
  ~bitarray(){
    if(p) delete [] p;
  }
  uint* p;
  uint blocknum;
  uint bitnum;
  
  bool get(uint i){
    uint x = i / ((sizeof(uint)*CHAR_BIT));
    uint y = i % ((sizeof(uint)*CHAR_BIT));
    return (p[x] & (uint)(1 << y))!=0;
  }
  void set(uint i){
    uint x = i / ((sizeof(uint)*CHAR_BIT));
    uint y = i % ((sizeof(uint)*CHAR_BIT));
    if ((p[x] & (uint)(1 << y)) == 0){
      p[x] |= (1 << y);
    }
  }
} barray;

uint AnalysizeData(uint *head, size_t sz,uint& mi, uint& ma){
  //timer t(__FUNCSIG__);
  uint *end = head + sz;
  uint *tmp = head;
  int i = 0;
  long double mean = 0;
  while (head < end){
    if (mi > *head)mi = *head;
    if (ma < *head)ma = *head;
    mean = mean * i / (i + 1.f) + *head / (i + 1.0f);
    ++i;
    ++head;
  }
  return (uint)mean;
}

#if 0
bool verify1(uint* head, uint* tail){
  for (uint i = 0; i < tail - head;i++){
    if (head[i] >= head[i + 1]){
      cout << head[i] << " " << head[i + 1] << endl;
      return false;
    }
  }
  return true;
}
#endif


/// radix sort 2.806s
// return size of the bucket
// 1,3,5,10,9,1
int AdaptiveSort(uint* head, uint* tail) {
  if (head==tail){
    return 1;
  }
  uint mi=UINT_MAX, ma=0;
  uint m=AnalysizeData(head, tail - head + 1,mi,ma);
  uint hsize = tail - head + 1;
  
  uint range = ma - mi - 1;
  //cout << "hsize:" << hsize << " span:" << range << endl;
  if (range > (1 << 27)){
    if (hsize < (1 << 21)){
      timer td("std sort");
      //std::sort
      std::sort(head, tail + 1);//0.891
      //copy(head, tail+1, ostream_iterator<int>(cout, " ")); cout << endl;
      return unique(head, tail + 1) - head;
    }else{
      //timer t("partition");
      //mypartition(head, tail, 16);
      uint* p = partition(head, tail + 1, bind2nd(less<uint>(), m));
      int siz1 = AdaptiveSort(head, p-1);
      int siz2 = AdaptiveSort(p, tail);
      memmove_s(head + siz1, (hsize - siz1)*sizeof(uint), p, siz2*sizeof(uint));
      return siz1 + siz2;
      //copy(head, tail + 1, ostream_iterator<int>(cout, " ")); cout << endl;
    }
  }else{
    timer ts("counting sort");
    barray ba(range);
    for (uint i = 0; i < hsize; ++i){
      if (head[i] - mi > 0){
        ba.set(head[i] - mi - 1);
      }
    }
    uint j = 0;
    head[j++] = mi;
    for (uint i = 0; i < ba.bitnum; ++i){
      if (ba.get(i)){
        head[j++] = mi + i+1;
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
    bkt(bkt_),bktsz(bktsz_), rdata(rdata_),rdatasz(rdatasz_), ptree(NULL)
  {
    ptree = new int[bktsz]();
  }

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
#if 0
        cout << tail - head << endl;
        for (int x = 0; x < bktsz-1;x++)
        {
          if (bkt[x+1]-bkt[x]!=1){
            cout << "wrong" << endl;
          }
        }
#endif
        if (tail - head == *tail - *head){
          num += target - *head + 1 + (head-headbackup);
          return true;
        }else{
          if (fuzzy && head + 1 == tail && *head<target && *tail>target){
            num += head - headbackup + 1;
            return false;
          }
          mid = head + ((tail - head + 1) >> 1);
          if (target == *head){
            num += head - headbackup + 1;
            return true;
          }else if (target == *tail){
            num += tail - headbackup + 1;
            return true;
          }else if (target == *mid){
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
    //printf("%d\n",bkt[bktsz-1]);
    for (uint i = 0; i < rdatasz - 1; i += 2){
      if (rdata[i] == rdata[i + 1]){
        ++umi[rdata[i]];
      }else{
        uint n1=0;
        _GetBktNoInSortedArray(bkt, bkt + bktsz - 1, rdata[i],n1);
        uint n2=n1-1;
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
