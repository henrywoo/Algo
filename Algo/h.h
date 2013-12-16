#include "ss.h"
#include "bst.h"
#include "list.h"
#include "util.h"
#include "tick.h"
#include <memory>//shared_ptr
#include "sort.h"
#include "algo.h"
#include "deque.h"
#include "stack.h"
#include "SkipListTmpl.h"
#include "searching.h"
#include "bitops.h"
#include "mt.h"
#include "graph.h"
#include "heap.h"
#include "numbercrunching.h"
#include "msinterview.h"
#include <complex>
#include "longestrange.h"//algo
#include "radixsort.h"
#include "asl.h"
#include <new>
#include "mymap.h"
#include "anagrams.h"
#include "Valid_Parentheses.h"
#include "triangle.h"
#include "boggle.h"
#include "matrix.h"
#include "boggle2.h"
#include "bigint.h"
#include "sorting.h"
#include "threesum.h"
#include "DisjointSet.h"
#include "htable.h"

#include "permutation.h"
#include "printring.h"
#include "hmm.h"
#include <forward_list>
#include <initializer_list>
#include <tuple>
#include <future>
#include "smartptr.h"
#include <thread>
#include "rpn.h"
#include "numOfPointInLineIn2DPlane.h"

int ref1 = 10;
class AAA{
public:
  int& ref = ref1;
  int a = 60;
  int b = 8;
  static const int i = 10;
  //AAA():a(ref1+1){ cout << a << endl; }
  AAA() :a(ref + 1){ cout << a << endl; }

  AAA& operator=(const AAA&);
  const AAA& operator+(const AAA&) const;
  const AAA& operator+(int);// const;
  //AAA(AAA&&){ cout << endl; }
};

/*void whichoneillegal(){
  AAA x, y, z;
  x = x = z + y;
  (z = x + x) = y + z;
  //x = y + z + 5;
  x = y + 5;
  x = x + 5 + z;
  cout << AAA::i << x.a << endl;
}*/