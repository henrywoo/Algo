//address space layout
#include "asl.h"
#include "mt.h" // CRITICAL_SECTION

namespace ASL{//address space layout 地址空间布局

    using namespace std;
    int global_dummy = 0;

    void checkASL(void *src){
        int local_dummy = 0;//in stack memory
        if( (unsigned long)src < (unsigned long )&local_dummy ){
            //global initialized data
            if(  (unsigned long )src <  (unsigned long )&global_dummy ){
                printf(".txt section\n\n");
            }else if (  (unsigned long ) src > (unsigned long ) &global_dummy){
                printf(".heap section\n\n");
            }
        }else{
            printf(".stack section\n\n");
        }
    }

    classA operator+(int a,const classA& b){
        return b+a;
    }

    void test(){

        classA aa;
        classA bb;
        classA cc=aa+1; // call different operator+ function
        classA dd=1+aa;

        char *a = "hello world";
        char b[100] = "hello world";
        //char *c = static_cast<char*>(malloc(100));
        //char* c=new char[100];
        void* c=malloc(100);
        //strcpy(c,a);

        printf("char *a = \"hello world\";\n");
        checkASL(a);                                        /* @1 */
        printf("char b[100] = \"hello world\";\n");
        checkASL(b);                                        /* @2 */
        printf(" (\"hello world\" )\n");
        checkASL("hello world" );                           /* @3 */
        printf("char *c = malloc(100);\n");
        checkASL(c);                                /* @4 */
        free(c);
        //delete [] c;

        ///////////////////////////////////////////////////////
        classA* ca1=new classA;
        classA* ca2=new (ca1) classA;
        // call constructor of classA at memory address ca1, and let ca2 point to ca1
        // placement new
        ca2->~classA();
        ::operator delete(ca1);// operator delete ==> free

        classA ca3;
        classA* ca4=new (&ca1) classA;

        //ca->~classA();
        //delete [4]  buf;
        //HeapAlloc()
        const int N=static_cast<int>(2);// 1.6G
        try{
            classA* a=new classA[N];
            delete [] a;
        }catch(std::bad_alloc& ba){
            std::cerr << "bad_alloc caught: " << ba.what() << "\n";
        }
        //MMSection::test();
        /*cout << hex << &argv << endl;
        cout << hex << argv << endl;
        cout << hex << *argv << endl;
        cout << hex << &argc << endl;*/
        cout << hex << &cs << endl;
        InitializeCriticalSection(&cs);
        cout << hex << &cs << endl;
        EnterCriticalSection(&cs);
        // shared resources
        cout << hex << &cs << endl;
        LeaveCriticalSection(&cs);
        DeleteCriticalSection(&cs);
        cout << hex << &cs << endl;
        int* pi=new int[4];
        pi[0]=0;
        pi[1]=1;
        cout << hex << &pi << endl;
        cout << hex << pi << endl;
        cout << hex << pi+1 << endl;
        delete [] pi;
    }

}