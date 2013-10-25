#include "std_function.h"
#include "std_bind.h"
#include <iterator>
#include <ppl.h>
#include <chrono>
#include "profile.h"
#include <thread>
#include <mutex>
#include <unordered_set>
#include <iostream>
#include "multi_producer_consumer.h"

using namespace std;
struct Complex final{
  std::recursive_mutex mutex;
  int i;
  Complex() : i(0) {}
  Complex(int) = delete;
  void mul(int x){
    std::lock_guard<std::recursive_mutex> lock(mutex);
    i *= x;
  }
  void div(int x){
    std::lock_guard<std::recursive_mutex> lock(mutex);
    i /= x;
  }
  virtual void both(int x, int y) final{
    std::lock_guard<std::recursive_mutex> lock(mutex);
    mul(x);
    div(y);
  }
};


int main(int argc, char* argv[]){
  mpc::test();
  char* d = R"(
string
\\
Simon
)";

  cout << d << endl;
  mutex m1, m2, m3;
  lock(m1,m2,m3);

  m1.unlock();
  m2.unlock();
  m3.unlock();
  string s = "sasdas";
  for (char c : s){
    cout << c << endl;
  }

  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i){
    threads.push_back(std::thread([](){
      std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
    }));
  }
  for (auto& thread : threads){
    thread.join();
  }

  profile();

  /*std::ratio<1, 19> r;
  cout << r.num << "/" << r.den << endl;*/

  int a[] = {1,2,3};
  auto x=begin(a);
  cout << *x << endl;
  std_function::test();
  std_bind::test();
  return 0;
}