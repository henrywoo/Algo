#include "ss.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include "bst.h"
#include "list.h"
#include <string>
#include <algorithm>
#include "util.h"
#include "tick.h"
#include <memory>//shared_ptr
#include <time.h>
#include "sort.h"
#include "algo.h"
#include "deque.h"
#include "stack.h"
//#include "ziplist.h"
//#include "zipmap.h"
#include "SkipListTmpl.h"
#include "searching.h"
#include "bitops.h"
#include "mt.h"
#include "graph.h"
#include "heap.h"
#include "numbercrunching.h"
#include "msinterview.h"
#include <thread>
#include <complex>
#include <queue>
#include "longestrange.h"//algo
#include "radixsort.h"
#include "asl.h"
#include <new>
//#include <boost/graph/adjacency_list.hpp>
using namespace std;
int foo(){
    int x;
    _asm mov x,1000b
    printf("%d\n",x);
}
string processpercentsign(const string& sss){
    string r(sss);
    size_t st=-2;
    //When pos is specified, the search only includes characters at or after position pos
    while(( st = r.find('%',st+2))!=string::npos){
        // find return The position of the first character of the first match.
        string s1=r.substr(0,st);
        s1.push_back('%');
        string s2=r.substr(st,r.size()-s1.size()+1);
        r=s1+s2;
    }
    return r;
}
///@note negative number is one more than positive number!! -128(-2**7) and 127(2**7-1); 2**31-1 and -2**31


//using namespace boost;
int main(int argc,char* argv[]){
    ASL::test();
    sorting::test();
    cout << boolalpha << algo::test_GetLongestRange() << endl;
    int x=5;
    ++++x;
    cout << x << endl;

#if 0
    x++++;//left operand must be l-value
#endif


    cout << boolalpha << testpower() << endl;
#if 0
    wstring hello=L"巫富珩";
    wcout<< hello.c_str() << endl;
#endif
    string sss="hello%20world%simon";
    string r=processpercentsign(sss);
    printf(r.c_str());
    printf("\n");
    printf("%s\n",r.c_str());
    cout << r.c_str() << endl;

    ss::test();
    heaping::test();
    bst::test();
    bt::test();
    MS::drawCircle(10);
    DP::test();
    getchar();
    statistics::test();
    //cout << boolalpha << bool(true^true) << endl;// xor can be used on bool
    foo();
    
    Augmentation::test();

    heaping::heap::test();
    int i=014;
    bitops::test();
    cout << bitops::BitCount(90) << endl;

    ::graph::test();

    std::thread t(hello);
    t.join();
    UINT tid;
    uintptr_t ut=::_beginthreadex(0,0,threadfunc,0,0,&tid);

    //WaitForSingleObject()
    cout << GetThreadId((HANDLE)ut) << endl;
    searching::test();
    //cout<< 0111010101010b << endl;
	exotic_datastructure::test();
#if 0
	adjacency_list<> mygraph;
	add_edge(1, 2, mygraph);
	add_edge(1, 3, mygraph);
	add_edge(1, 4, mygraph);
	add_edge(2, 4, mygraph);
	adjacency_list<>::vertex_iterator vertexIt, vertexEnd;
	adjacency_list<>::adjacency_iterator neighbourIt, neighbourEnd;
#endif
	__asm{int 3}

	//bst::test();
	//dclist::test();
	slist::test();
	//util::test();
	//tickseries::test();
	//sandbox::heap<double> h;
	//h.insert(0.2);

	//heap::test();
	//sorting::test();
	//DP::test();
	//deque2::test();
	//wufuheng::stackwithminmax<double>::test();
	//vector<int> v;v.back();
	//Augmentation::test();
	//sorting::test();
	//addTwoArbitraryNumbers_::test();
	//integerrelated::test();
#if 0
	int i=0x80000000;
	cout << i << endl;
	i>>=1;
	cout << i << endl;
	cout << hex << i << endl;
	char a='a';
	char b='b';
	util::swap<char>(a,b);
	cout << a << b << endl;
#endif
	//stringrelated::test();
	//integerrelated::test();
	return getchar();
}

