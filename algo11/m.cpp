#include "robot.h"
#include <process.h>

#define USINGCOUNTINGSORT

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
public:
  BigTest(){
  }

  ~BigTest(){
  }


  //return # of int
  uint GetExtents(const char* filename){
    timer t(__FUNCSIG__ " Read File");
    SYSTEM_INFO sinf;
    GetSystemInfo(&sinf);
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, 
      OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    DWORD dwFileSizeHigh;
    __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
    qwFileSize += (((__int64)dwFileSizeHigh) << 32);
    CloseHandle(hFile);

    __int64 qwFileOffset = 0, qwNumOf0s = 0;
    uint remain = 0;
    DWORD dwBytesInBlock = sinf.dwAllocationGranularity*1<<10;

    uint siz = dwBytesInBlock > qwFileSize ? qwFileSize : dwBytesInBlock / 5 * 2;
    uint *phead = new uint[siz];//0 0\r\n
    uint *ptail = phead;

    vector<uint*> vures;

    int residual = -1;
    vector<HANDLE*> hs;
    while (qwFileSize > 0) {
      // Determine the number of bytes to be mapped in this view
      if (qwFileSize < dwBytesInBlock)
        dwBytesInBlock = (DWORD)qwFileSize;
      PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ, (DWORD)(qwFileOffset >> 32), // Starting byte
        (DWORD)(qwFileOffset & 0xFFFFFFFF), // in file
        dwBytesInBlock); // # of bytes to map 
      if (residual>=0){
        *ptail++ = residual;
        residual = -1;
      }
      FastScan(ptail, pbFile, dwBytesInBlock, remain);
      //printf("%d %d %d %d\n", *tmp, *(tmp - 1), *(tmp - 2), *(tmp - 3));
      vector<int> rdata(phead, ptail);
      size_t sz = rdata.size();
      if (sz&1){
         residual = rdata.back();
         rdata.pop_back();
         --sz;
      }
      vector<int> bkt(rdata);
#ifdef USINGCOUNTINGSORT
      {// can be optimized here min max
        timer _t("counting sort");
        int ma = INT_MIN, mi=INT_MAX;
        for (int i = 0; i < sz; ++i){
          if (ma < bkt[i]){
            ma = bkt[i];
          }
          if (mi > bkt[i]){
            mi = bkt[i];
          }
        }
        counting_sort(bkt, ma);
      }
#else
      // counting sort here
      {
        timer _t; sort(bkt.begin(), bkt.end());
        bkt.erase(unique(bkt.begin(), bkt.end()), bkt.end());
      }
#endif
      Robot rbt(bkt, rdata);
      {
        timer t;
        rbt.BuildBIT();
        /*unsigned thrdaddr;
        hs.push_back((HANDLE)::_beginthreadex(NULL, NULL, submerge, &pir[i], NULL, &thrdaddr));*/
      }
      vr.push_back(rbt);

      ptail = phead;
      UnmapViewOfFile(pbFile);
      qwFileOffset += dwBytesInBlock;
      qwFileSize -= dwBytesInBlock;
    }
    CloseHandle(hFileMapping);// 2.3 seconds debug
    return(ptail-phead);
  }

  uint Query(uint target){
    uint r=0;
    for (int i = 0; i < vr.size(); i++){
      r += vr[i].query(target);
    }
    return r;
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
        //int n = __query(qint);
        int n = Query(qint);
#if NDEBUG
        of.put(n);
#else
        printf("%d\t%d\n", qint, n);
#endif
      }
    }
    if (stream)fclose(stream);
  }

  struct ofile{
    FILE* o;
    ofile() :o(NULL){ fopen_s(&o, "output5.txt", "w"); }
    ~ofile(){ fclose(o); }
    void put(int i){ o != NULL && fprintf_s(o, "%d\n", i); }
  };

  vector<Robot> vr;

};

void gen(){
  ofstream f("randompair2.txt", std::ofstream::out | std::ofstream::app);
  time_t start = time(0);
  srand((unsigned int)time(NULL));
  for (int i = 0; i<50 * 1000000; ++i){
    int t1 = rand() % 212343;
    int t2 = rand();
    if (t1>t2){
      swap(t1, t2);
    }
    f << t1 << " " << t2 << endl;
  }
  time_t end = time(0);
}

int main(int argc, char* argv[]){
  //gen(); exit(0);
  /*vector<int> a = {0,0,2,1,3,1,2};
  counting_sort(a,  6);
  uint x[6] = {};
  char y[] = "121 221\n321 90\n434334 9890\n";
  FastScan(x, (PBYTE)y, _countof(y),0);*/

  timer t(__FUNCSIG__);
  BigTest bt;
  if (1){
    uint u=bt.GetExtents("randompair2.txt");
    //uint u = bt.GetExtents("extents.txt");
    //uint u = bt.GetExtents("test.txt");
    bt.QueryFromFile("numbers.txt");
    //bt.QueryFromFile("q.txt");
  }
  return 0;
}