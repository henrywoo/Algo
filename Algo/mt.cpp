#include "mt.h"
#include <algorithm>
#include <time.h>
CRITICAL_SECTION cs; //definition of global var

UINT WINAPI threadfunc(LPVOID){
    while (true){
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

typedef struct{
    int* h;
    int* t;
    int ordinal;
    HANDLE* hevent;
} intrange;

unsigned int __stdcall submerge(void* p){
    intrange* pir=static_cast<intrange*>(p);
    //SetEvent(*pir->hevent);
    srand(time(NULL));
    int nap=rand()%10;
    Sleep(nap*1000);// mil seconds
    cout << "thread id:" << pir->ordinal << endl;
    return 0;
}

void mtmerge(int* head, int* tail, int threadnum){
    void *security=NULL;
    unsigned stacksize=NULL;
    unsigned createflag=NULL;
    unsigned thrdaddr;
    int interval=(tail-head)/threadnum;
    intrange* pir=new intrange[threadnum];
    HANDLE* hs=new HANDLE[threadnum];
    for (int i=0;i<threadnum;++i){
        pir[i].ordinal=i;
        //pir[i].hevent=&hs[i];
        //todo
        ///@link http://msdn.microsoft.com/en-us/library/kdzttdcb%28v=vs.90%29.aspx
        hs[i]=(HANDLE)::_beginthreadex(security,stacksize,submerge,
            &pir[i],createflag,&thrdaddr);
    }
#if 0
    DWORD d=WaitForMultipleObjects(threadnum,hs,true,INFINITE);
    switch(d){
    case WAIT_OBJECT_0:
        cout << "All threads are done!" << endl;
        for_each(hs,hs+threadnum,CloseHandle);
        break;
    default:
        printf("Wait error: %d\n", GetLastError());
    }
#else
    int tmp=threadnum;
    while(tmp){
        //(WAIT_OBJECT_0 + nCount– 1)
        // If bWaitAll is TRUE, the return value indicates that the 
        //state of all specified objects is signaled.
        // If bWaitAll is FALSE, the return value minus WAIT_OBJECT_0 
        //indicates the lpHandles array index of the object that
        //satisfied the wait.
        //*If more than one object became signaled during the call, 
        //this is the array index of the signaled object with the 
        //SMALLEST index value of all the signaled objects.*//
        DWORD d=WaitForMultipleObjects(threadnum,hs,false,INFINITE);
        /////variables cannot be declared in a switch statement
        switch(d){
        case WAIT_OBJECT_0:{cout << "thread0 is done!" << endl;tmp--;break;}
        case WAIT_OBJECT_0+1:{cout << "thread1 is done!" << endl;tmp--;break;}
        case WAIT_OBJECT_0+2:{cout << "thread2 is done!" << endl;tmp--;break;}
        case WAIT_OBJECT_0+3:{cout << "thread3 is done!" << endl;tmp--;break;}
        case WAIT_OBJECT_0+4:{cout << "thread4 is done!" << endl;--tmp;break;}
        case WAIT_OBJECT_0+5:{cout << "thread5 is done!" << endl;--tmp;break;}
        case WAIT_OBJECT_0+6:{cout << "thread6 is done!" << endl;--tmp;break;}
        case WAIT_OBJECT_0+7:{cout << "thread7 is done!" << endl;--tmp;break;}
        case WAIT_OBJECT_0+8:{cout << "thread8 is done!" << endl;--tmp;break;}
        case WAIT_OBJECT_0+9:{cout << "thread9 is done!" << endl;--tmp;break;}
        default:
            printf("Wait error: %d\n", GetLastError());
            break;
        }
    }
    
    DWORD d2=WaitForMultipleObjects(threadnum,hs,true,INFINITE);
    switch(d2){
    case WAIT_OBJECT_0:
        int i=0;
        cout << "All threads are done!" << endl;
        //for_each(hs,hs+threadnum,CloseHandle);
        while(threadnum--) CloseHandle(hs[threadnum]);
        break;
    }
#endif
}

int islst( ){
    ULONG Count;
    PSLIST_ENTRY pFirstEntry, pListEntry;
    PSLIST_HEADER pListHead;
    PPROGRAM_ITEM pProgramItem;

    // Initialize the list header to a MEMORY_ALLOCATION_ALIGNMENT boundary.
    pListHead = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER),
        MEMORY_ALLOCATION_ALIGNMENT);
    if( NULL == pListHead )
    {
        printf("Memory allocation failed.\n");
        return -1;
    }
    InitializeSListHead(pListHead);
    // Insert 10 items into the list.
    for( Count = 1; Count <= 10; Count += 1 ){
        pProgramItem = (PPROGRAM_ITEM)_aligned_malloc(sizeof(PROGRAM_ITEM),
            MEMORY_ALLOCATION_ALIGNMENT);
        if( NULL == pProgramItem )
        {
            printf("Memory allocation failed.\n");
            return -1;
        }
        pProgramItem->Signature = Count;
        pFirstEntry = InterlockedPushEntrySList(pListHead, 
            &(pProgramItem->ItemEntry)); 
    }

    // Remove 10 items from the list and display the signature.
    for( Count = 10; Count >= 1; Count -= 1 ){
        pListEntry = InterlockedPopEntrySList(pListHead);

        if( NULL == pListEntry ){
            printf("List is empty.\n");
            return -1;
        }

        pProgramItem = (PPROGRAM_ITEM)pListEntry;
        printf("Signature is %d\n", pProgramItem->Signature);

        // This example assumes that the SLIST_ENTRY structure is the 
        // first member of the structure. If your structure does not 
        // follow this convention, you must compute the starting address 
        // of the structure before calling the free function.

        _aligned_free(pListEntry);
    }

    // Flush the list and verify that the items are gone.
    pListEntry = InterlockedFlushSList(pListHead);
    pFirstEntry = InterlockedPopEntrySList(pListHead);
    if (pFirstEntry != NULL){
        printf("Error: List is not empty.\n");
        return -1;
    }

    _aligned_free(pListHead);

    return 1;
}

// A surely deadlock example
class gard_mutex:public guard{
public:
    gard_mutex(){
        h=CreateMutex(NULL,FALSE,NULL);
    }
    ~gard_mutex(){
        CloseHandle(h);
    }
    void lock(){
        WaitForSingleObject(h,INFINITE);
    }
    void unlock(){
        ReleaseMutex(h);
    }
};


//global var
HANDLE ghMutexA;
HANDLE ghMutexB;

// thread func
unsigned int __stdcall transfer1(void *p){
    DWORD da=WaitForSingleObject(ghMutexA,INFINITE);
    cout << "thread1: da=" << da << endl;
    if (da==WAIT_OBJECT_0){
        cout << "" << endl;
    }
    Sleep(2000);
    DWORD db=WaitForSingleObject(ghMutexB,INFINITE);
    cout << "transfer1" << endl;
    ReleaseMutex(ghMutexB);
    CloseHandle(ghMutexB);
    ReleaseMutex(ghMutexA);
    CloseHandle(ghMutexA);
    return 0;
}

unsigned int __stdcall transfer2(void *p){
    DWORD db=WaitForSingleObject(ghMutexB,INFINITE);
    Sleep(2000);
    DWORD da=WaitForSingleObject(ghMutexA,INFINITE);
    cout << "transfer2" << endl;
    ReleaseMutex(ghMutexA);
    CloseHandle(ghMutexA);
    ReleaseMutex(ghMutexB);
    CloseHandle(ghMutexB);
    return 0;
}

#define NOT_OWNED false
//If you are using a named mutex to limit your application to a single instance, a malicious user can create this mutex before you do and prevent your application from starting. To prevent this situation, create a randomly named mutex and store the name so that it can only be obtained by an authorized user. Alternatively, you can use a file for this purpose. To limit your application to one instance per user, create a locked file in the user's profile directory.
void ShowMeDeadLock(){
    //The state of a mutex object is signaled when it is not owned by any thread.
    ghMutexA=CreateMutex(NULL,NOT_OWNED,NULL);
    ghMutexB=CreateMutex(NULL,NOT_OWNED,NULL);
    _beginthreadex(NULL,0,transfer1,NULL,NULL,NULL);
    //_beginthreadex(NULL,0,transfer2,NULL,NULL,NULL);

    DWORD db=WaitForSingleObject(ghMutexB,INFINITE);//LOCK
    cout << "main thread: db=" << db << endl;
    Sleep(2000);
    DWORD da=WaitForSingleObject(ghMutexA,INFINITE);//LOCK
    cout << "transfer2" << endl;
    ReleaseMutex(ghMutexA);//UNLOCK
    ReleaseMutex(ghMutexB);//UNLOCK
}

//volatile 
bool Sentinel = true;
int CriticalData = 0;

unsigned ThreadFunc1( void* pArguments ) {
    while (Sentinel)
        Sleep(0);   // volatile spin lock

    // CriticalData load guaranteed after every load of Sentinel
    cout << "Critical Data = " << CriticalData << endl;
    return 0;
}

unsigned  ThreadFunc2( void* pArguments ) {
    Sleep(2000);
    CriticalData++;   // guaranteed to occur before write to Sentinel
    Sentinel = false; // exit critical section
    return 0;
}

int TestVolatile() {
    HANDLE hThread1, hThread2;
    DWORD retCode;
    hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadFunc1,NULL, 0, NULL);
    hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadFunc2,NULL, 0, NULL);
    if (hThread1 == NULL || hThread2 == NULL){
        cout << "CreateThread failed." << endl;
        return 1;
    }
    retCode = WaitForSingleObject(hThread1,3000);
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    if (retCode == WAIT_OBJECT_0 && CriticalData == 1 )
        cout << "Success" << endl;
    else
        cout << "Failure" << endl;
}