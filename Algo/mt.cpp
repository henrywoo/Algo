#include "mt.h"

CRITICAL_SECTION cs; //definition of global var

UINT WINAPI threadfunc(LPVOID){
    cout << "hello" << endl;
    while (true)
    {
        Sleep(10000);
    }
    return 0;
}


void hello(){
    std::cout<<"Hello Concurrent World\n";
}


void test(){
    // global variable in a process
    EnterCriticalSection(&cs);
    // shared resources
    LeaveCriticalSection(&cs);
}