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
#include <iomanip>

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

//using namespace boost;
template<typename T>
class Foo{
public:
  //Foo(){}
  Foo(T){}
};
class Foo2 : public Foo<string>{
public:
  //Foo2(){}
};
//Foo2 f;
namespace{ static int an = 8; }
namespace A{
  namespace{ static int an2=7; }
}
using namespace A;
//------------------------------------------
template<typename T> class DA{
  T* c;
  int sz;
public:
  explicit DA(int){}
};
DA<int> a1(7);
DA<int> a2('7');//def contor
//DA<int> a3=7; // constructor
DA<int> a4(a1);//cpycotor
DA<int> a5=a2;//cpycotor
//------------------------------------------
#define PPP cout << __FUNCSIG__ << endl;
namespace{
  struct A{
    A(){ PPP }
    //A(const A&) = delete;
    virtual void f(){ PPP }
  };
  struct B: public A{
    B(){PPP}
    virtual void f(){ PPP }
  };
  void e1(){
    try{
      throw B();
    }catch (A ex){
      ex.f();
    }catch (B& ex){
      ex.f();
    }
  }
}
//------------------------------------------
namespace{
  bool hasintersection(vector<int>& v1, vector<int>& v2){
    auto a=find_first_of(v1.begin(), v1.end(), v2.begin(), v2.end());
    if (a != v1.end()){
      cout << *a << endl;
      return true;
    }else{
      return false;
    }
  }
  void e2(){
    vector<int> v1 = { 1, 2, 3 };
    vector<int> v2 = { 11, 12, 2 };
    cout << hasintersection(v1, v2) << endl;
  }
}
//------------------------------------------
namespace ns1{
  class A{
    int n;
    void virtual f() throw(int, double, long){}
  };
  struct B: public A{
    float n;
    //int virtual f() throw(double, int, long){}
    void virtual f() throw(double, int, long){
      PPP
    }
    //void f() throw(string){}
    //void f(){}
  };

  void e3(){
    B b;
    b.f();
  }
}
//------------------------------------------
namespace ns2{
  void t(){
    vector<string> vs(1,"|");
    fill_n(back_inserter(vs), 5, "h");
    fill_n(inserter(vs,vs.begin()), 5, "e");
    //generate_n(vs.begin(), 5, "h");
  }
}
//////////////////////////////////////////////////////////////////////////
namespace ns3{
  struct A{ void n(){} void n2() const{} };
  void test(){
    const A a;
    //a.n();// const object cannot call non-const function!!
    a.n2();
  }
}
//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
  ns2::t();
  ns1::e3();
  cout << hex << 18 << setbase(10) << 18<< endl;
  cout << hex << showbase << 18 << endl;
  e1();
  e2();
  cout << an << endl;
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