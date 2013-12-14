#include <memory>
#include <new>
#include <iostream>
#include <assert.h>
#include <map>
#include <functional>
namespace{
//#include <>
///@brief assignment
class Apple{
public:
  //unique_ptr<int[]> data;
  int* data;
  size_t sz;
  mutable int weight;

  Apple(){}
  ~Apple(){
    //
  }
  Apple(const Apple& a){}

  ///@brief assignment operator
  Apple& operator=(const Apple& a/*ref*/)
  {
    if (&a == this){
      return *this;
    }
    // previous data
    if (data) delete[] data;

    // deep copy, allocate new heap
    data = new (std::nothrow) int[a.sz];
    if (data){
      memcpy_s(data, a.sz*CHAR_BIT, a.data, a.sz*CHAR_BIT);
    }
    return *this;
  }

  //prefix increment
  Apple& operator++()
  {
    this->weight += 1;
    return *this;
  }

  //postfix increment -> call prefix increment
  Apple operator++(int)
  {
    Apple b=*this;
    ++b;
    return b;
  }



};


// fixed length
// 0000..0100 [len=32]
// 4 -> bin(4) = 0100
void printBinary(unsigned int i){
  assert(i >= 0);
  size_t sz = sizeof(i)* CHAR_BIT; // 4
  int k = 0;
  for (size_t j = 1; j <= sz; ++j){ // j=1, 2
    k = 1 << (sz-j);//sz-j=3, k=0b1000; sz-j=2,k=0b0100, i=0b0100;
    // doesn't work for negative int
    std::cout << ((i & k) >> (sz-j)); //i&k=0x0100; (i&k)>>(2)=1
  }// 0 1
  std::cout << std::endl;
}

//-1 >> 1 => become +0b0111111111..11 (wrong)
//coz the sign bit has never been shifted
//[0b(1)111111...1111] -> -1 

//signed int -1 -> (0b(1)111111...1111)
void printBinary3(int i){
  if (i >= 0){
    printBinary(i);
  }else{// -1 0b(1)1 0b(1)111111...1110+1 [0b(1)111111...1111] -> -1
    std::cout << 1;
    int i2 = -i;
    i2 = ~i2 + 1;// complement and plus 1
    size_t sz = sizeof(i2)* CHAR_BIT; // 4
    int k = 0;
    for (size_t j = 2; j <= sz; ++j){ // j=1, 2
      k = 1 << (sz - j);//sz-j=3, k=0x1000; sz-j=2,k=0x0100, i=0x0100;
      std::cout << ((i2 & k) >> (sz - j)); //i&k=0x0100; (i&k)>>(2)=1
    }// 0 1
    std::cout << std::endl;
  }
}

// not fixed length
// 4 - 0x100  -> 001 [len=3]
void printBinary2(unsigned int i){
  assert(i >= 0);
  size_t sz = sizeof(i)* CHAR_BIT; // 32
  while (i){
    i >>= 1;
    std::cout << (i&1);
  }
  std::cout << std::endl;
}
// ^ & | ~

//functor 
struct l2{
  //state
  bool operator()(const Apple& a, const Apple& b){ return true;/*todo*/ }
};
std::map<Apple, int, l2> m;//specialization
std::map<Apple, int, std::less<Apple>> m2;

// 1. functor -> adaptor/adaptation 2. inline 3. save the state more flexible
// jmp to the function stored in other places
// functor -> inlined -> embeded code -> higher performance

// c++11
//auto_ptr is obsolete -> replaced by unique_ptr

//shared_ptr
class mysharedptr{
  int rc;
  int* data;

  mysharedptr* next;
  mysharedptr* prev;

  //circular linked list - > bookkeeping of all the owners of the data block
  mysharedptr* sentinel;// add this to tail of list
public:
  mysharedptr():rc(0),data(nullptr){}
  mysharedptr(int* p){//initialization with a raw pointer
    if (p){
      rc++;
      if (data){
        //todo
      }else{
      }
    }
  }
  //pimp pointer to implementation
  // copy m
  mysharedptr(const mysharedptr& m){
    if (m.getrc()>0)// if m is null
    {}
  }

  int* getd() const{
    return data;
  }
  int getrc() const{
    return 1;
  }
};


class RApple : public Apple{
  //adfasd
};

void func(){
  try{
    throw &RApple();
  }catch (Apple* r){//obj slicing
    //r->func();
    throw r; // it will copy the value of the pointer ,not the obj pointed
    //asdfsd
  }//best practice
  catch (Apple& a){
  }
}

}

