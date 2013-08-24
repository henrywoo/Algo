#include "mt.h"
#include <algorithm>
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

#include <time.h>


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
    for( Count = 1; Count <= 10; Count += 1 )
    {
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
    for( Count = 10; Count >= 1; Count -= 1 )
    {
        pListEntry = InterlockedPopEntrySList(pListHead);

        if( NULL == pListEntry )
        {
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
    if (pFirstEntry != NULL)
    {
        printf("Error: List is not empty.\n");
        return -1;
    }

    _aligned_free(pListHead);

    return 1;
}