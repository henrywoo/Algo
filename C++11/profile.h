#include <vector>
#include <deque>
#include <time.h>
#include <iostream>

using namespace std;

void profile(){
  std::vector<int> vecint;
  std::deque<int> queint;

  clock_t t0 = 0, t1 = 0;

  int size = 1000000;

  t0 = clock();
  for (int i = 0; i < size; ++i)
  {
    vecint.push_back(i);
  }
  t1 = clock();
  //vector的push_back速度，共350ms，毕竟有realloc和copy的过程
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < size; ++i)
  {
    queint.push_back(i);
  }
  t1 = clock();
  //而deque因为没有realloc和copy的过程，只用了340ms，其实也没快多少
  //不过要是push_front，就会快非常多了
  std::cout << t1 - t0 << std::endl;

  int i;

  t0 = clock();
  for (auto it = vecint.begin(); it != vecint.end(); ++it)
  {
    i = *it;
  }
  t1 = clock();
  //用iterator遍历vector，花费882ms，不推荐这么遍历
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (auto it = queint.begin(); it != queint.end(); ++it)
  {
    i = *it;
  }
  t1 = clock();
  //用iterator遍历deque，花费768ms，对于deque这已经是最快的方法了
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < size; ++i)
  {
    i = vecint[i];
  }
  t1 = clock();
  //用下标遍历vector，花费55ms，推荐的方式
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < size; ++i)
  {
    i = queint[i];
  }
  t1 = clock();
  //用下标遍历deque，花费1548ms，比iterator方式慢了一倍
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (unsigned int i = 0; i < vecint.size(); ++i)
  {
    i = vecint[i];
  }
  t1 = clock();
  //一个小细节，即便使用下标遍历，最好把size提前取出来，
  //每次访问一下还是要牺牲性能的，花费了82ms。
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    int* p = new int;
    delete p;
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    std::unique_ptr<int> p(new int);
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    std::shared_ptr<int> p=make_shared<int>();
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    //std::shared_ptr<int> p(new int);
    std::shared_ptr<int> p(new int[20],
      [](int* p){delete[] p; }
    );
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    char* p = new char[20];
    delete[] p;
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;

  t0 = clock();
  for (int i = 0; i < 1000000; ++i)
  {
    std::unique_ptr<char[]> p(new char[20]);
  }
  t1 = clock();
  std::cout << t1 - t0 << std::endl;
}
