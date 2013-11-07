#include "robot.h"
#include <process.h>

#define USINGCOUNTINGSORT

///@return true - full read; false - incomplete read
///@todo add function to deal with \n only linux file
void FastAdaptiveScan(uint*& des,PBYTE src,DWORD slen,uint& remain){
  PBYTE endpos = src + slen -1;
  while (*src == ' ' || *src == '\n' || *src == '\r'){
    if (*src == ' ' || *src == '\r'){
      *des++ = remain;
      remain = 0;
    }
    src++;
  }// remove non-digit character
  register uint val = remain;
  register bool sawreturn = false;//for linux format
  while (src <= endpos) {
    //'\n' is the conventional ending on Unix machines, '\r' was (I think) used on old Mac operating systems, and Windows uses a pair, '\r' following by '\n'.
    if (*src == ' '){
      *des++ = val;
      val =0;
      src++;
    }else if (*src == '\r'){ // && *(src + 1) == '\n'
      *des++ = val;
      val =0;
      ++src;
      sawreturn = true;
    }else if (*src=='\n'){
      if (sawreturn){
        sawreturn = false;
      }else{
        *des++ = val;
        val =0;
      }
      ++src;
    }
    else
      val = val * 10 + (*src++ - '0');
  }
  remain = val;
}
#include <list>

list<uint*> resourcepointers;
Robot* vr[1<<20] = {};//1M robots
int robotsz = 0;

class BigTest{
public:
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
    DWORD dwBytesInBlock = sinf.dwAllocationGranularity*(1<<10);

    uint ByteNum = dwBytesInBlock > qwFileSize ? qwFileSize : dwBytesInBlock;

    uint *phead = new uint[ByteNum/5*2];//0 0\r\n
    uint *ptail = phead;

    vector<uint*> vures;

    int residual = -1;
    uint bktsz;
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

      FastAdaptiveScan(ptail, pbFile, dwBytesInBlock, remain);
      //printf("%d %d %d %d\n", *tmp, *(tmp - 1), *(tmp - 2), *(tmp - 3));
      int rdatasz = ptail - phead;
      if (rdatasz & 1){
        residual = *--ptail;
        --rdatasz;
      }
      uint* rdata = new uint[rdatasz];

      for (int i = 0; i < rdatasz; ++i){
        if (phead[i] == 133050368){
          cout << "GG" << endl;
        }
      }

      resourcepointers.push_back(rdata);//rdata
      memcpy_s(rdata, rdatasz*sizeof(uint), phead, rdatasz*sizeof(uint));
      {
        timer _tss("counting sort");
        bktsz = AdaptiveSort(phead, ptail);
        if (!verify1(phead, phead + bktsz-1))
          cout << "wrong" << endl;
        for (int i = 0; i < bktsz;++i){
          if (phead[i] == 133050368){
            cout << "GG" << endl;
          }
        }
      }
      uint* bkt = new uint[bktsz];
      resourcepointers.push_back(bkt);//bucket
      memcpy_s(bkt, bktsz*sizeof(uint), phead, bktsz*sizeof(uint));
      Robot* rbt = new Robot(bkt, bktsz, rdata, rdatasz);
      {
        timer t;
        rbt->BuildBIT();
        /*unsigned thrdaddr;
        hs.push_back((HANDLE)::_beginthreadex(NULL, NULL, submerge, &pir[i], NULL, &thrdaddr));*/
      }
      vr[robotsz++] = rbt;

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
    for (int i = 0; i < robotsz; i++){
      r += vr[i]->query(target);
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

  

};

void gen(){
  ofstream f("randompair3.txt", std::ofstream::out | std::ofstream::app);
  srand((unsigned int)time(NULL));
  for (int i = 0; i<50 * 1000* 1000; ++i){
    uint t1 = rand() << i;
    uint t2 = rand() << (i+sizeof(int)-2);
    if (t1>t2){
      swap(t1, t2);
    }
    f << t1 << " " << t2 << endl;
  }
  exit(0);
}



int main(int argc, char* argv[]){
  //gen();
  timer t(__FUNCSIG__);
  BigTest bt;
  if (1){
    RAND_MAX;
    //uint u = bt.GetExtents("extents2.txt");
    //uint u=bt.GetExtents("test.txt");
    //uint u = bt.GetExtents("extents.txt");
    uint u = bt.GetExtents("randompair3.txt");
    //bt.QueryFromFile("numbers.txt");
    bt.QueryFromFile("q.txt");
  }
  return 0;
}