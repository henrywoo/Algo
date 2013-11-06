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

using namespace std;
typedef unsigned int uint;
#define MAXLINE 51*1000*1000

struct timer{
  int start;
  const char* str;
  timer(const char* s=""):start(clock()),str(s){}
  ~timer(){ printf("[%s] Time Cost: %.3lf\n", str, double(clock() - start) / CLOCKS_PER_SEC); }
};

///@return true - full read; false - incomplete read
///@todo add function to deal with \n only linux file
void FastScan(uint*& des,PBYTE src,DWORD slen,uint& remain){
  PBYTE endpos = src + slen -1;
  while (*src == ' ' || *src == '\n' || *src == '\r'){
    if (*src == ' ' || *src == '\r'){
      *des++ = remain;
      remain = 0;
    }
    src++;
  }// remove non-digit character
  int val = remain;
  while (src <= endpos) {
    //'\n' is the conventional ending on Unix machines, '\r' was (I think) used on old Mac operating systems, and Windows uses a pair, '\r' following by '\n'.
    if (*src == ' '){
      *des++ = val;
      val ^= val;
      src++;
    }else if (*src == '\r'){ // && *(src + 1) == '\n'
      *des++ = val;
      val ^= val;
      ++++src;
    }else
      val = val * 10 + (*src++ - '0');
  }
  remain = val;
}

class BigTest{
private:
  int* ptree;//int array
  uint* numbers;
  int* bucket;
  vector<vector<int>> bucket2;
  uint rcount;
  uint ucount;
  unordered_map<uint, uint> umi;
  enum { CAPACITY = MAXLINE * 2 };
protected:
  void __update(uint first, uint second){
    uint i = first, tmp = 0;
    while (i >= 0 && i <= ucount){
      tmp = i - 1;
      ++ptree[tmp];
      i += (i & -(int)i);
    }
    i = second;
    while (i >= 0 && i <= ucount){
      tmp = i - 1;
      --ptree[tmp];
      i += (i & -(int)i);
    }
  }

  uint __query(uint target){
    uint idx = __getbucketno(target, 0, false);
    uint sum = 0, tmp = 0;
    while (idx > 0){
      tmp = idx - 1;
      sum += ptree[tmp];
      idx -= (idx&-(int)idx);
    }
    if (umi.find(target) != umi.end()){
      sum += umi[target];
    }
    return sum;
  }



  //binary search O(logN) - todo
  uint __getbucketno(int target, uint lo = 0, bool tagging = true){
    uint hi = ucount - 1, mid;
    if (tagging){
      while (lo <= hi){
        mid = lo + (hi - lo) / 2;
        if (bucket[mid] == target)
          return mid + 1;
        else if (bucket[mid] < target)
          lo = mid + 1;
        else
          hi = mid - 1;
      }
    }
    else{
      while (lo < hi){
        if (target >= bucket[hi]) return hi + 1;
        if (lo + 1 == hi && bucket[lo] < target && target < bucket[hi]){
          return lo + 1;
        }
        mid = lo + (hi - lo) / 2;
        if (bucket[mid] == target)
          return mid + 1;
        else if (bucket[mid] < target)
          lo = mid;
        else
          hi = mid;
      }
    }
    return -1;// not found
  }

  void __debug(){
#ifdef DEBUG
    for (int i = 0; i < ucount; ++i){
      cout << ptree[i] << endl;
    }
    cout << "***********************" << endl;
#endif // DEBUG
  }

public:
  BigTest():rcount(0),bucket(NULL){
    ptree = new int[CAPACITY]();
  }

  ~BigTest(){
    if (ptree) delete ptree;
    if (bucket)delete bucket;
  }

  void __preprocess(int n){
    rcount = n;
    timer t(__FUNCSIG__);
    bucket = new int[rcount];
    memcpy(bucket, numbers, rcount * sizeof(int));
    /*{timer r;
    set<int> si(bucket, bucket + count * 2 + 1); // 9 seconds
    }*/
    sort(bucket, bucket + rcount);//0.8 seconds
    int *p = unique(bucket, bucket + rcount);
    //cout << p - bucket << endl;// size of the unique array
    /*for (int i = 0; bucket + i != p;++i){
    cout << bucket[i] << endl;
    }*/
    ucount = p - bucket;
    for (uint i = 0; i < rcount - 1; i = i + 2){
      if (numbers[i] == numbers[i + 1]){
        ++umi[numbers[i]];
      }
      else{
        uint n1 = __getbucketno(numbers[i]);
        uint n2 = __getbucketno(numbers[i + 1], n1 - 1);
        ++umi[numbers[i + 1]];
        __update(n1, n2);
        //debug();
      }
    }
    delete numbers;
    __debug();
  }

  //return # of int
  uint GetExtents(const char* filename){
    timer t(__FUNCSIG__ " Read File");
    SYSTEM_INFO sinf;
    GetSystemInfo(&sinf);
    HANDLE hFile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    DWORD dwFileSizeHigh;
    __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
    qwFileSize += (((__int64)dwFileSizeHigh) << 32);
    CloseHandle(hFile);

    uint* tmp = numbers = new uint[CAPACITY];


    __int64 qwFileOffset = 0, qwNumOf0s = 0;
    uint remain = 0;
    while (qwFileSize > 0) {
      // Determine the number of bytes to be mapped in this view
      DWORD dwBytesInBlock = sinf.dwAllocationGranularity;
      if (qwFileSize < sinf.dwAllocationGranularity)
        dwBytesInBlock = (DWORD)qwFileSize;
      PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ, (DWORD)(qwFileOffset >> 32), // Starting byte
        (DWORD)(qwFileOffset & 0xFFFFFFFF), // in file
        dwBytesInBlock); // # of bytes to map 

      FastScan(tmp, pbFile, dwBytesInBlock, remain);


      //printf("%d\n", tmp - numbers);

      // Unmap the view; we don't want multiple views
      // in our address space.
      UnmapViewOfFile(pbFile);
      // Skip to the next set of bytes in the file.
      qwFileOffset += dwBytesInBlock;
      qwFileSize -= dwBytesInBlock;
    }
    CloseHandle(hFileMapping);// 2.3 seconds debug

    return(tmp-numbers);
    /*
    FILE *stream=NULL;
    errno_t err = freopen_s(&stream, filename, "r", stdin);
    if (err==0){
      // read file
      {
        timer t(__FUNCSIG__ " Read File");
        //[bool __thiscall BigTest::GetExtents(const char *) Read File] Time Cost: 224.450, MM 800M
        int* tmp = numbers = new int[CAPACITY];
        while (scanf_s("%d %d\n", tmp, tmp + 1) != EOF){
          ++++tmp;
        }
        if (stream) fclose(stream);
        rcount = tmp - numbers;
      }
      __preprocess();
      return true;
    }else
      return false;
    */
  }

  

  void QueryFromFile(const char* filename){
    FILE *stream = NULL;
    errno_t err = freopen_s(&stream, filename, "r", stdin);
    if (err == 0){
      int qint;
#if NDEBUG
      ofile of;
#endif
      while (scanf_s("%d\n", &qint) != EOF){
        int n = __query(qint);
#if NDEBUG
        of.put(n);
#else
        printf("%d\n", n);
#endif
      }
    }
    if (stream)fclose(stream);
  }

  struct ofile{
    FILE* o;
    ofile() :o(NULL){ fopen_s(&o, "output.txt", "w"); }
    ~ofile(){ fclose(o); }
    void put(int i){ o != NULL && fprintf_s(o, "%d\n", i); }
  };

};



int main(int argc, char* argv[]){

  /*uint x[6] = {};
  char y[] = "121 221\n321 90\n434334 9890\n";
  FastScan(x, (PBYTE)y, _countof(y),0);*/

  timer t;
  BigTest bt;
  if (1){
    uint u=bt.GetExtents("randompair.txt");
    bt.__preprocess(u);
    bt.QueryFromFile("numbers.txt");
  }
  return 0;
}