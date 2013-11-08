#include <algorithm>
#include <unordered_map>
#include <windows.h>
#include <process.h>
#include <functional>

#define SINGLEROBOTNUM 1
#define RESMAXNUM (1<<15)
#define UINT_BIT (sizeof(UINT)*CHAR_BIT)
#define MINLINESZ _countof("0 0\n")
#define NUMBEROFGRANULARITY (1<<10)
#define MULTITHREADTHRESHOLD (600 * 1024 * 1024)
#define MAXRANGEFORCOUNTINGSORT (1<<27)
#define STDSORTTHRESHOLD (1<<21)

using namespace std;

class Robot;
Robot* vr[RESMAXNUM] = {};
HANDLE hs[RESMAXNUM] = {};
UINT* resourcepointers[RESMAXNUM] = {};
UINT robotsz = 0;
UINT resoursz = 0;

typedef struct {
  UINT* rdata;
  UINT rdatasz;
  Robot** prbt;
} threaddata;

// Sorting Algorithm
///////////////////////////////////////////////////////////////
///@brief bit array for counting sort
typedef struct bitarray{
  explicit bitarray(UINT range) :p(NULL),
  blocknum(range / (UINT_BIT)+1),
  bitnum(blocknum*(UINT_BIT))
  {
    if (range > 0){ p = new UINT[blocknum](); }
  }
  ~bitarray(){
    if (p) delete[] p;
  }
  UINT* p;
  UINT blocknum;
  UINT bitnum;

  bool get(UINT i){
    UINT x = i / ((UINT_BIT));
    UINT y = i % ((UINT_BIT));
    return (p[x] & (UINT)(1 << y)) != 0;
  }
  void set(UINT i){
    UINT x = i / ((UINT_BIT));
    UINT y = i % ((UINT_BIT));
    if ((p[x] & (UINT)(1 << y)) == 0){
      p[x] |= (1 << y);
    }
  }
} barray;

///@brief introspective analysis for counting sort without overflow
UINT AnalysizeData(UINT *head, size_t sz, UINT& mi, UINT& ma){
  UINT *end = head + sz;
  UINT *tmp = head;
  int i = 0;
  long double mean = 0;
  while (head < end){
    if (mi > *head)mi = *head;
    if (ma < *head)ma = *head;
    mean = mean * i / (i + 1.f) + *head / (i + 1.0f);
    ++i;
    ++head;
  }
  return (UINT)mean;
}

///@brief sort integers using std:sort or counting sort adaptively
///@return size of the bucket
int AdaptiveSort(UINT* head, UINT* tail) {
  if (head == tail){
    return 1;
  }
  UINT mi = UINT_MAX, ma = 0;
  UINT hsize = tail - head + 1;
  UINT m = AnalysizeData(head, hsize, mi, ma);
  UINT range = ma - mi - 1;
  if (range > (1 << 27)){
    if (hsize < (1 << 21)){
      std::sort(head, tail + 1);
      return unique(head, tail + 1) - head;
    }
    else{
      UINT* p = partition(head, tail + 1, bind2nd(less<UINT>(), m));
      int siz1 = AdaptiveSort(head, p - 1);
      int siz2 = AdaptiveSort(p, tail);
      memmove_s(head + siz1, (hsize - siz1)*sizeof(UINT), p, siz2*sizeof(UINT));
      return siz1 + siz2;
    }
  }
  else{
    barray ba(range);
    for (UINT i = 0; i < hsize; ++i){
      if (head[i] - mi > 0){
        ba.set(head[i] - mi - 1);
      }
    }
    UINT j = 0;
    head[j++] = mi;
    for (UINT i = 0; i < ba.bitnum; ++i){
      if (ba.get(i)){
        head[j++] = mi + i + 1;
      }
    }
    return j;
  }
}

///////////////////////////////////////////////////////////////
///@brief Robot class implemented as a BIT(binary indexed tree)
class Robot{
  int* ptree;
  UINT* rdata;
  UINT rdatasz;
  UINT* bkt;
  UINT bktsz;

  unordered_map<UINT, UINT> umi;

  UINT __query_BIT(UINT idx){
    if (idx == UINT_MAX){
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

  void __update_BIT(UINT first, UINT second){
    UINT i = first, tmp = 0;
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

  ///@param target - query integer
  ///num [in|out] - bucket number(num == UINT_MAX means "not in any bucket")
  ///fuzzy - the query integer must be a boundary point(false) or in-between(true)
  ///@return true - target is a boundary point
  bool __getbucketposition(UINT* head, UINT* tail, UINT target, UINT& num, bool fuzzy = false){
    if (target < *head || target>*tail){
      num = UINT_MAX;
      return false;//edge case
    }
    else{
      UINT* headbackup = head;
      UINT* mid = 0;
      //binary search optimized for big data
      while (head < tail){
        if (tail - head == *tail - *head){
          num += target - *head + 1 + (head - headbackup);
          return true;
        }
        else{
          if (fuzzy && head + 1 == tail && *head<target && *tail>target){
            num += head - headbackup + 1;
            return false;
          }
          mid = head + ((tail - head + 1) >> 1);
          if (target == *head){
            num += head - headbackup + 1;
            return true;
          }
          else if (target == *tail){
            num += tail - headbackup + 1;
            return true;
          }
          else if (target == *mid){
            num += mid - headbackup + 1;
            return true;
          }
          else if (target > *mid){
            head = mid;
          }
          else{
            tail = mid;
          }
        }
      }
    }
    throw 1;//will never be here, just to disable compiler warning
  }

public:
  explicit Robot(UINT* bkt_, UINT bktsz_, UINT* rdata_, UINT rdatasz_) :
    bkt(bkt_), bktsz(bktsz_), rdata(rdata_), rdatasz(rdatasz_), ptree(NULL){
    ptree = new int[bktsz]();
  }

  ~Robot(){
    if (!ptree) delete[] ptree;
  }

  void BuildBIT(){
    for (UINT i = 0; i < rdatasz - 1; i += 2){
      if (rdata[i] == rdata[i + 1]){
        ++umi[rdata[i]];
      }
      else{
        UINT n1 = 0;
        __getbucketposition(bkt, bkt + bktsz - 1, rdata[i], n1);
        UINT n2 = n1 - 1;
        __getbucketposition(bkt + n1 - 1, bkt + bktsz - 1, rdata[i + 1], n2);
        __update_BIT(n1, n2);
      }
    }
  }

  UINT Query(UINT target){
    UINT idx = 0;
    bool isboundrypoint = __getbucketposition(bkt, bkt + bktsz - 1, target, idx, true);
    UINT m1 = __query_BIT(idx);
    if (isboundrypoint){
      UINT m2 = 0;
      if (idx > 1)
        m2 = __query_BIT(idx - 1);
      int r = m2 > m1 ? m2 : m1;
      if (umi.find(target) != umi.end()){
        r += umi[target];
      }
      return r;
    }
    else{
      return m1;
    }
  }

};

///@brief scan and load number from buffer
///'\n' is the conventional ending on Unix machines;
///'\r' was (I think) used on old Mac operating systems
///Windows uses a pair, '\r' following by '\n'.
void FastAdaptiveScan(UINT*& des, PBYTE src, DWORD slen, UINT& remain){
  PBYTE endpos = src + slen - 1;
  while (*src == ' ' || *src == '\n' || *src == '\r'){
    if (*src == ' ' || *src == '\r'){
      *des++ = remain;
      remain = 0;
    }
    src++;
  }// remove non-digit character
  register UINT val = remain;
  register bool sawreturn = false;//for Linux format
  while (src <= endpos) {
    if (*src == ' '){
      *des++ = val;
      val = 0;
      src++;
    }
    else if (*src == '\r'){
      *des++ = val;
      val = 0;
      ++src;
      sawreturn = true;
    }
    else if (*src == '\n'){
      if (sawreturn){
        sawreturn = false;
      }
      else{
        *des++ = val;
        val = 0;
      }
      ++src;
    }
    else if (*src > '0' - 1 && *src < '9' + 1){
      val = val * 10 + (*src++ - '0');
    }
    else{
      return;
    }
  }
  remain = val;
}

///////////////////////////////////////////////////////////////
///@brief Multi-thread Function Body, one thread -> one robot
unsigned int __stdcall WorkerThreadFunc(void* p){
  threaddata* d = static_cast<threaddata*>(p);
  UINT* tmp = new UINT[d->rdatasz];
  memcpy_s(tmp, d->rdatasz*sizeof(UINT), d->rdata, d->rdatasz*sizeof(UINT));
  UINT bktsz = AdaptiveSort(tmp, tmp + d->rdatasz - 1);
  UINT*pp = unique(tmp, tmp + bktsz);
  UINT* bkt = new UINT[bktsz];
  resourcepointers[resoursz] = bkt;
  InterlockedExchangeAdd(&resoursz, 1);
  memcpy_s(bkt, bktsz*sizeof(UINT), tmp, bktsz*sizeof(UINT));
  delete[] tmp;
  *d->prbt = new Robot(bkt, bktsz, d->rdata, d->rdatasz);
  (*d->prbt)->BuildBIT();
  return 0;
}

///@brief load extends file and do all preprocessing work
///multi-threaded adaptively according to CPU core # and size of input data
bool GetExtents(const char* filename){
  SYSTEM_INFO sinf;
  GetSystemInfo(&sinf);
  HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
    OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (hFile == INVALID_HANDLE_VALUE){
    printf("[ERROR]Cannot read file %s\n", filename);
    return false;
  }
  DWORD dwFileSizeHigh;
  __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
  qwFileSize += (((__int64)dwFileSizeHigh) << 32);
  CloseHandle(hFile);
  if (qwFileSize == 0){
    printf("[ERROR]Empty input file %s\n", filename);
    return false;
  }

  bool smalldata = qwFileSize < sinf.dwAllocationGranularity;

  HANDLE hFileMapping = NULL;
  PBYTE fbuffer = NULL;
  FILE* pFile = NULL;
  hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
  if (!smalldata){
    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
  }
  __int64 qwFileOffset = 0, qwNumOf0s = 0;
  UINT remain = 0;
  DWORD dwBytesInBlock = sinf.dwAllocationGranularity*NUMBEROFGRANULARITY;
  UINT ByteNum = (UINT)(dwBytesInBlock > qwFileSize ? qwFileSize : dwBytesInBlock);
  UINT *phead = new UINT[(ByteNum / MINLINESZ) << 1];
  UINT *ptail = phead;
  int residual = -1;

  while (qwFileSize > 0){
    if (qwFileSize < dwBytesInBlock)
      dwBytesInBlock = (DWORD)qwFileSize;
    if (!smalldata){
      fbuffer = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ,
        (DWORD)(qwFileOffset >> 32), // Starting byte
        (DWORD)(qwFileOffset & 0xFFFFFFFF), // in file
        dwBytesInBlock); // # of bytes to map 
    }
    else{
      fbuffer = new unsigned char[(UINT)qwFileSize];
      fopen_s(&pFile, filename, "rb");
      fread_s(fbuffer, (UINT)qwFileSize, 1, (UINT)qwFileSize, pFile);
    }

    if (residual >= 0){
      *ptail++ = residual;
      residual = -1;
    }
    FastAdaptiveScan(ptail, fbuffer, dwBytesInBlock, remain);
    int rdatasz = ptail - phead;
    if ((ptail - phead) & 1){
      residual = *--ptail;
      --rdatasz;
    }
    UINT* rdata = new UINT[rdatasz];
    resourcepointers[resoursz] = rdata;//rdata
    InterlockedExchangeAdd(&resoursz, 1);
    memcpy_s(rdata, rdatasz*sizeof(UINT), phead, rdatasz*sizeof(UINT));
    if ((sinf.dwNumberOfProcessors < 2) ||
      ((qwFileSize < MULTITHREADTHRESHOLD) && (robotsz < SINGLEROBOTNUM)))
    {//single thread
      UINT bktsz = AdaptiveSort(phead, ptail);
      UINT* bkt = new UINT[bktsz];
      resourcepointers[resoursz] = bkt;//bucket
      InterlockedExchangeAdd(&resoursz, 1);
      memcpy_s(bkt, bktsz*sizeof(UINT), phead, bktsz*sizeof(UINT));
      Robot* rbt = new Robot(bkt, bktsz, rdata, rdatasz);
      rbt->BuildBIT();
      vr[robotsz++] = rbt;
    }
    else{ //multi-thread
      threaddata* pdata = new threaddata;
      pdata->rdata = rdata;
      pdata->rdatasz = rdatasz;
      pdata->prbt = &vr[robotsz];
      hs[robotsz++] = ((HANDLE)::_beginthreadex(NULL, NULL, WorkerThreadFunc, pdata, NULL, NULL));
    }

    ptail = phead;
    if (smalldata){
      delete[] fbuffer;
    }
    else{
      UnmapViewOfFile(fbuffer);
    }
    qwFileOffset += dwBytesInBlock;
    qwFileSize -= dwBytesInBlock;
  }
  CloseHandle(hFileMapping);
  if (robotsz > SINGLEROBOTNUM){
    //printf("%d num of threads\n", robotsz - SINGLEROBOTNUM);
    WaitForMultipleObjects(robotsz - SINGLEROBOTNUM, hs + SINGLEROBOTNUM, true, INFINITE);
  }
  return true;
}

///@brief Query interface
void QueryFromFile(const char* filename, const char* ofname = NULL){
  FILE *istm = NULL, *ostm = NULL;
  errno_t err = freopen_s(&istm, filename, "r", stdin);
  if (err != 0){
    printf("[ERROR]Cannot read file %s\n", filename);
    return;
  }
  if (ofname){
    err = fopen_s(&ostm, ofname, "w");
    if (err != 0){
      printf("[ERROR]Cannot create file output file %s\n", ofname);
      return;
    }
  }
  UINT qint = 0;
  while (scanf_s("%d\n", &qint) != EOF){
    UINT result = 0;
    for (UINT i = 0; i < robotsz; i++){
      result += vr[i]->Query(qint);
    }
    (ostm != NULL) && fprintf_s(ostm, "%d\n", result);
    printf("%d\n", result);
  }
  if (istm)fclose(istm);
}

///@brief reclaim resources allocated
void CleanUp(){
  for (int i = 0; i < RESMAXNUM; ++i){
    if (vr[i]){ delete vr[i]; }
    if (hs[i]){ CloseHandle(hs[i]); }
    if (resourcepointers[i]){ delete[] resourcepointers[i]; }
  }
}

//#include <chrono>
//using namespace chrono;
int main(int argc, char* argv[]){
  //time_point<system_clock> start = high_resolution_clock::now();
  if (GetExtents("test.txt")){
    QueryFromFile("numbers.txt");
    CleanUp();
  }
  //printf("%d ms\n",duration_cast<milliseconds>(high_resolution_clock::now() - start).count());
  return 0;
}