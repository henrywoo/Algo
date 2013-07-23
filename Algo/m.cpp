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

#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;
int main(int argc,char* argv[]){

	adjacency_list<> mygraph;
	add_edge(1, 2, mygraph);
	add_edge(1, 3, mygraph);
	add_edge(1, 4, mygraph);
	add_edge(2, 4, mygraph);
	adjacency_list<>::vertex_iterator vertexIt, vertexEnd;
	adjacency_list<>::adjacency_iterator neighbourIt, neighbourEnd;


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

