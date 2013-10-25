#include <windows.h>
#include <process.h>
#include <synchapi.h>
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

namespace mtnolock{
    ///@link http://www.mitbbs.com/article_t/JobHunting/32494279.html
}

UINT WINAPI threadfunc(LPVOID);

void mtmerge(int* head, int* tail, int threadnum=10);

void hello();

///@link http://stackoverflow.com/questions/10422034/when-to-use-extern-in-c
// any source file that includes this will be able to use "cs"
extern CRITICAL_SECTION cs;

void test();

// interlocked single linked list
typedef struct _PROGRAM_ITEM {
    SLIST_ENTRY ItemEntry;
    ULONG Signature; 
} PROGRAM_ITEM, *PPROGRAM_ITEM;

int islst();

void ShowMeDeadLock();
int TestVolatile();

template<class T>
class guard{
protected:
    T h;
public:
    guard(const T& lhs):h(lhs){}
    virtual ~guard(){}
    virtual void lock()=0;
    virtual void unlock()=0;
};