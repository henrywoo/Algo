#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <malloc.h> 
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class ExBase{
public:
  virtual void what(){
    cout << "ExBase\n";
  }
};
class ExDerv:public ExBase{
public:
  void what(){
    cout << "ExDerv\n";
  }
};

//A->B achieve this goal
// 1. 
class B{
public:
  //B(){}
  //B(const A&){}
  //B(const B&){}
};
class A{
public:
  //operator B(){} // conversion operator to convert A to B
  //void f();
  operator B();
};
//void A::f(){}
A::operator B(){
  cout << "implicit conversion" << endl;
  return B();
}

void fun(B b){}
void test(){
  A a;
  fun(a);// how to make this valid call
}

namespace AA{
  struct X{};
  void g(X){ cout << "I am here." << endl; }
}

// new/delete are operators, which you can not overload;
// what does new/delete do compared with malloc and free(in c)?
// two steps by which new allocates the memeory in c++:
// 1. allocate the memory according to the size // call a operator new,which can be overloaded
// 2. to instantiate the object and fit it in the memeory allocated

class N{
private:
  struct M{
    M():i(0){
      cout << __FUNCSIG__ << endl;
    }
    int i; 
  } m;
public:
  N() { cout << "CTOR" << endl; }
  ~N(){ cout << "DTOR" << endl; }
  int get(){ return m.i; }

  // variadic parameter
  void * operator new(size_t sz, string s1, string s2){
    cout << __FILE__ << __FUNCTION__   << endl;
    cout << s1.c_str() << "\t" << s2.c_str() << endl;
    return ::operator new(sz);
  }

  void * operator new(size_t sz, char* cs){
    cout << "Log:" << cs << endl;
    return ::operator new(sz);
  }

  void operator delete(void* p){
    cout << __FILE__ << __FUNCTION__ << endl;
    //operator delete(p);// call N::operator delete
    ::operator delete(p);//global scope resolution operator
  }
};

int main(int argc, char* argv[]){
  cout << __FUNCSIG__ << endl;
  cout << sizeof N << endl;
  N* pn;
  size_t pnvalue;
  {
    //memory location in stack
    char pc[] = "world";// pc is in stack, but world is in DATA
    int i(~(1<<31));
    char *pc2 = "hello";// non-const pointer but const data

    pn = new (pc) N;
    delete pn;
    // we want to construct the object of N in pc
    // reuse the memory in stack, so a new return a stack mem...interesting!
    pn = ::new(pc) N; // you must use global new // cannot use pc2
    cout << pn->get() << endl;
    pn = ::new(&i) N;
    cout << pn->get() << endl;
    pnvalue = reinterpret_cast<size_t>(pn);
  }
  // hacker way
  int j(12);
  if (pnvalue == reinterpret_cast<size_t>(pn))
    cout << pn->get() << endl;
  else{
    cout << "pn changed!" << endl;
  }

  test();
  cout << boolalpha << (true ^ false) << endl;
  cout << boolalpha << true << endl;
  cout << true << endl;
  AA::X x1;
  g(x1);
  //Koenig lookup only apply to namespace, not class
  try{
    throw ExDerv();
  }catch (ExBase& e){
    e.what();
  }catch (ExDerv& e){
    e.what();
  }catch (...){}
  char a[2] = {};
  strcpy_s(a,2,"a");
  int address;
  char *q;
  char *p = (char *)malloc(sizeof(char)* 1);
  printf("%d\n",strlen("1"));
  q = strcpy(p, "1");
  printf("p:%s(%p),q:%s(%p)\n", p, p, q, q);
  address = (int)p;
  printf("int:%p", address);
  printf("the string is %s\n", (char *)address);

  p = (char *)malloc(sizeof(char)* 300000);
  free(p);
  //p++;
  return 0;
}