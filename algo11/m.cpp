#include<vector>
#include<utility>
#include<iostream>
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

void gen(){
  ofstream f("randompair.txt", std::ofstream::out | std::ofstream::app);
  time_t start = time(0);
  srand((unsigned int)time(NULL));
  for (int i = 0; i<5 *1000000; ++i){
    int t1 = rand() % 212343;
    int t2 = rand() % 23433723;
    if (t1>t2){
      swap(t1, t2);
    }
    f << t1 << " " << t2 << endl;
  }
  time_t end = time(0);
  cout << "Time elapse: " << end - start << endl;
}

struct timer{
  int start;
  timer():start(clock()){}
  ~timer(){ printf("%.3lf\n", double(clock() - start) / CLOCKS_PER_SEC); }
};

typedef pair<int, int> interval;
typedef pair<int, int> indices;


#define MAX 6
int T[MAX + 1] = {}; //Define a space for BIT
//function to update
void update(indices idx){
  int i = idx.first;
  while (i >= 0 && i <= MAX){
    //cout << i << endl;
    ++T[i];
    i += (i & -i );
  }

  i = idx.second;
  while (i >= 0 && i <= MAX){
    //cout << i << endl;
    --T[i];
    i += (i & -i);
  }
}
//function to query
int query(int idx){
  int sum = 0;
  while (idx > 0){
    sum += T[idx];
    idx -= (idx&-idx);
  }
  return sum;
}

int getBucketNo(const set<int>& p, int target){
  if (p.empty() || *p.begin()>target){
    return -1;
  }
  int bucketNo = 0;
  int j = 0;
  for (int k : p){
    if (target < k){
      break;
    }
    j++;
  }
  bucketNo = j;
  return bucketNo;
}

class BigTest{
private:
  int* ptree;//int array
  int* numbers;
  int* bucket;
  unordered_map<uint, uint> umi;
  uint rcount;
  uint ucount;


public:
  BigTest():rcount(0){
    ptree = new int[MAXLINE / 5 * 2];
    memset(ptree, 0, MAXLINE / 5 * 2 * sizeof (int));
    //bucket = new int[MAXLINE / 5 * 2];
  }

  ~BigTest(){
    delete ptree;
    delete bucket;
  }

  void update(int first, int second){
    int i = first, tmp=0;
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

  int query(int target){
    int idx = (int)FindBucketNo(target, 0, false);
    int sum = 0, tmp=0;
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

  bool getrawdata(const char* filename){
    numbers = new int[MAXLINE / 5 * 2];
    FILE *stream;
    {
      timer t;
      freopen_s(&stream, filename, "r", stdin);
      int* tmp = numbers;
      while (scanf_s("%d %d\n", tmp, tmp + 1, 20) != EOF){
        tmp += 2;
      }
      rcount = tmp - numbers;
    }
    /*ifstream inFile(filename);
    int lineNo=count(istreambuf_iterator<char>(inFile),istreambuf_iterator<char>(), '\n');
    cout << lineNo << endl;*/
    return true;
  }
  
  void debug(){
#ifdef DEBUG
    for (int i = 0; i < ucount;++i){
      cout << ptree[i] << endl;
    }
    cout << "***********************" << endl;
#endif // DEBUG
  }

  void preprocess(){
    timer t;
    bucket = new int[rcount];
    

    memcpy(bucket, numbers, rcount * sizeof(int));
    /*{timer r;
      set<int> si(bucket, bucket + count * 2 + 1); // 9 seconds
    }*/
    sort(bucket, bucket + rcount);//0.8 seconds
    int *p = unique(bucket, bucket + rcount);
    cout << p - bucket << endl;// size of the unique array
    /*for (int i = 0; bucket + i != p;++i){
      cout << bucket[i] << endl;
    }*/
    ucount = p - bucket;
    for (uint i = 0; i < rcount - 1; i = i + 2){
      if (numbers[i] == numbers[i+1]){
        ++umi[numbers[i]];
      }else{
        uint n1 = FindBucketNo(numbers[i]);
        uint n2 = FindBucketNo(numbers[i + 1], n1 - 1);
        ++umi[numbers[i+1]];
        update(n1, n2);
        //debug();
      }
    }
    debug();
  }

  //binary search O(logN)
  uint FindBucketNo(int target, uint lo=0, bool tagging=true){
    uint hi = ucount - 1, mid;
    if (tagging){
      while (lo <= hi){
        mid = lo + (hi - lo) / 2;
        if (bucket[mid] == target)
          return mid+1;
        else if (bucket[mid] < target)
          lo = mid+1;
        else
          hi = mid-1;
      }
    }else{
      while (lo < hi){
        if (target >= bucket[hi]) return hi + 1;
        if (lo+1==hi && bucket[lo] < target && target < bucket[hi]){
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

};

int main(void){
  int a[]={16,16,17,18,18,19};
  int *p=unique(a,a+_countof(a));
  cout << p-a << endl;
  
#if 0
  gen();
  exit(0);
#endif
  BigTest bt;
#if 1
  bt.getrawdata("extents.txt");
#else 
  bt.getrawdata("test.txt");
#endif
  bt.preprocess();
  int n = bt.query(102731);
  cout << n << endl;
  for (int i = 300; i < 350; ++i){
    int n = bt.query(i);
    cout << i << "\t" << n << endl;
  }
  return 0;
}