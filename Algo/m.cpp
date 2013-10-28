#include "h.h"

//You need to add the Python include directory (e.g. c:\python33\include) 
//to the list of include paths. You'll need to do the same for the library
//directory (e.g. c:\python33\libs)
//#define BOOST_PYTHON_STATIC_LIB
//#include <boost/python.hpp>

//#pragma comment(lib, "boost_python-vc110-mt-gd-1_54.lib")
//#include <boost/graph/adjacency_list.hpp>
using namespace std;
///@note negative number is one more than positive number!!
//-128(-2**7) and 127(2**7-1); 2**31-1 and -2**31
struct AAA{
	int a;
	AAA(){cout<<endl;}
	//AAA(AAA&&){ cout << endl; }
};

template<typename T> struct is_int{
    static const bool ISINT = false; 
}; 
template<> struct is_int<int>{
    static const bool ISINT = true;
};

void simon(){
    static int ccc;
    static int ddd=100;
    static AAA aaa;
    ccc=10;
    ddd=1;
}
template<class T> class A{
public:
  A(){}
}; 
template<class T> class B{
  int x = 10;
public:
    B() {};
    //friend class A<T>;
    //template friend class A<T>;
    /* add something here to make A a friend */
};
static void func1(){
  cout << "func1" << endl;
}

//using namespace boost;
template<typename...>
class newtemp{};

#include <initializer_list>
#include <tuple>
#include <future>

void funct() throw(int){
  int i = 0;
  throw 0.1;// as long as we can catch it with (...) for example.
}

int main(int argc,char* argv[]){
  bt::test();
  char bbg[] = "dogcat";
  recursion::permute(bbg);
  recursion::permute(bbg,0,_countof(bbg));
  green_turtle::test();
  heaping::heap::test();
  int *p = (int*)malloc(sizeof(int)*30000);
  free(p);
  try{
    funct();
  }catch (int i){
    cout << i << endl;
  }catch (double d){
    cout << d << endl;
  }catch (...){
    cout << "ERROR" << endl;
  }
  util::test();
  bst::test();
  
  cout << std::thread::hardware_concurrency() 
    << " threads will be the best!" << endl;
  int v2[] = {1,2};
  for (int x : v2){
    cout << x << endl;
  }
  std::async(std::launch::async, func1);
  vector<int> vx{1,2,3,4};
  vx.shrink_to_fit();
  vector<int>(vx).swap(vx);
  int tmp = vx.size();
  for_each(vx.begin(), vx.end(), [&](int x){cout << x; if (--tmp == 0){ cout << endl; }});
  { unique_ptr<int[]> uniquepoint(new int[5]); }
  int localvar = 0;
  [&localvar]()mutable->int{localvar++; return 1; }();
  
  bigint::test();
  anagrams::test();
  cout << std::thread::hardware_concurrency() << endl;
  cout << std::this_thread::get_id() << "\t" << endl;

  extern char nonstaticvar;
  extern void nonstaticfunc();

  cout << nonstaticvar << endl;
  nonstaticfunc();
  string ss("1234567890");
  vector<string> vs;
  std::fill_n(std::back_inserter(vs), 5, "hello");

  {
    shared_ptr<int> spi(new int(50));
    spi.reset(new int(100));
    //std::swap();
  }

  reverse_copy(ss.begin(), ss.end(), ostream_iterator<char>(cout));
  set<int> si;
  while (1){
    pair<set<int>::iterator, bool> tmp1 = si.insert(1);
    if (tmp1.second){
      cout << "inserted a new value " << *tmp1.first << ".\n";
      set<int>::iterator tmp2 = si.insert(tmp1.first, 2);
    }
    else{
      cout << "an equivalent element " << *tmp1.first << " exists already!" << endl;
      int a[] = { 2, 3, 4 };
      si.insert(a, a + _countof(a));
      break;
    }
  }
  copy(si.begin(), si.end(), ostream_iterator<int>(cout, " ")); cout << endl;

  simon();
  static int ccc = 0;
  ASL::test();
  bst::test();
  slist::test();
  TestVolatile();
  boggle::test();
  //set<set<int>> ssi;
  //unordered_set<set<int>> ussi;
  algo::threesum::test();
  //ShowMeDeadLock();
  sorting2::test();
  bigint::test();
  cout << boolalpha << testpower() << endl;

  for (int i = 2; i<100; i <<= 1){}
  MS::test1();
  MS::boggle2::test();
  matrix::test();
  boggle::test();
  triangle::test();
  cout << boolalpha <<
    stackapplication::IsValidParentheses("{([])}{}()") << endl;
  anagrams::test();
  bitops::test();
  mymap::test();
  sorting::test();
  bt::test();
  const int N = static_cast<int>(1e3);
  int* a = new int[N];
  for (int i = 0; i<N; i++){ a[i] = i; }
  mtmerge(a, a + N - 1);
  Sleep(10 * 1000);
  ASL::test();
  sorting::test();
  cout << boolalpha << algo::test_GetLongestRange() << endl;
  int x = 5;
  ++++x;
  cout << x << endl;

#if 0
  x++++;//left operand must be l-value
#endif

#if 0
  wstring hello=L"巫富珩";
  cout<< hello.c_str() << endl;
#endif
  string sss = "hello%20world%simon";
  string r = processpercentsign(sss);
  printf(r.c_str());
  printf("\n");
  printf("%s\n", r.c_str());
  cout << r.c_str() << endl;

  ss::test();
  heaping::test();

  bt::test();
  MS::drawCircle(10);
  DP::test();
  getchar();
  statistics::test();
  //cout << boolalpha << bool(true^true) << endl;// xor can be used on bool

  Augmentation::test();

  heaping::heap::test();
  int i = 014;

  cout << bitops::BitCount(90) << endl;

  ::graph::test();

  std::thread t(hello);
  t.join();
  UINT tid;
  uintptr_t ut = ::_beginthreadex(0, 0, threadfunc, 0, 0, &tid);

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
  //__asm{int 3}

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

