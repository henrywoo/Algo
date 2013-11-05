#include <utility>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <time.h>
#include <fstream>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;
typedef unsigned int uint;
#define MAXLINE 50*1000*1000

struct timer{
  int start;
  timer():start(clock()){}
  ~timer(){ printf("Time Cost: %.3lf\n", double(clock() - start) / CLOCKS_PER_SEC); }
};

void gen(){
  ofstream f("randompair.txt", std::ofstream::out | std::ofstream::app);
  time_t start = time(0);
  srand((unsigned int)time(NULL));
  for (int i = 0; i<5 * 1000000; ++i){
    int t1 = rand() % 212343;
    int t2 = rand() % 23433723;
    if (t1>t2){
      swap(t1, t2);
    }
    f << t1 << " " << t2 << endl;
  }
  time_t end = time(0);
}

class BigTest{
private:
  int* ptree;//int array
  int* numbers;
  int* bucket;
  uint rcount;
  uint ucount;
  unordered_map<uint, uint> umi;
  enum { CAPACITY = MAXLINE / 5 * 2 };
protected:
  void __update(int first, int second){
    int i = first, tmp = 0;
    while (i >= 0 && i <= ucount){
      tmp = i - 1;
      ++ptree[tmp];
      i += (i & -i);
    }
    i = second;
    while (i >= 0 && i <= ucount){
      tmp = i - 1;
      --ptree[tmp];
      i += (i & -i);
    }
  }

  int __query(int target){
    int idx = (int)__getbucketno(target, 0, false);
    int sum = 0, tmp = 0;
    while (idx > 0){
      tmp = idx - 1;
      sum += ptree[tmp];
      idx -= (idx&-idx);
    }
    if (umi.find(target) != umi.end()){
      sum += umi[target];
    }
    return sum;
  }

  void __preprocess(){
    timer t;
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
    __debug();
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
  BigTest():rcount(0){
    ptree = new int[CAPACITY]();
  }

  ~BigTest(){
    delete ptree;
    delete bucket;
  }

  bool GetExtents(const char* filename){
    FILE *stream;
    timer t;
    errno_t err = freopen_s(&stream, filename, "r", stdin);
    if (err==0){
      int* tmp = numbers = new int[CAPACITY];
      while (scanf_s("%d %d\n", tmp, tmp + 1, 20) != EOF){
        tmp += 2;
      }
      fclose(stream);
      rcount = tmp - numbers;
      __preprocess();
      return true;
    }else
      return false;
  }

  void QueryFromFile(const char* filename){
    FILE *stream;
    freopen_s(&stream, filename, "r", stdin);
    int qint;
    while (scanf_s("%d\n", &qint, 1) != EOF){
      int n = __query(qint);
      printf("%d\n",n);
    }
  }

};

int main(void){
  timer t;
#if 0
  gen();
  exit(0);
#endif
  BigTest bt;
#if 1
  bt.GetExtents("extents.txt");
#else 
  bt.GetExtents("test.txt");
#endif
  bt.QueryFromFile("numbers.txt");
  return 0;
}