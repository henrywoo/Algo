#ifndef __ALGO_ASL__
#define __ALGO_ASL__
//address space layout
#include <iostream>
/*static,automatic,register,dynamic*/

///Obsolete as windows 8 is using ASLR address space layout randomization
namespace ASL{

    class classA{
    private: int i;
    public:
        classA():i(0){
            std::cout << "constructed [" << this << "]\n";
        }
        void seti(int n){i=n;}
        //classA c=a+b;
        classA operator+(int a_) const{
            classA tmp;
            tmp.seti(this->i + a_);
            return tmp;
        }
        ~classA(){
            i=1;
        }
    };

    classA operator+(int a,const classA& b);

    void checkASL(void *src);

    /*
    // Linux Gave the expected answer, but in windows all are on stack
    [localhost] [root]:~/DistributedSystem
    $g++ main.cpp
    [localhost] [root]:~/DistributedSystem
    $./a.out 
    char *a = "hello world";
    .txt section

    char b[100] = "hello world";
    .stack section

    ("hello world" )
    .txt section

    char *c = malloc(100);
    .heap section
    */
    void test();

}


#endif

