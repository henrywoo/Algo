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

using namespace std;

int main(int argc,char* argv[]){
	bst::test();
	//dclist::test();
	//slist::test();
	//util::test();
	//tickseries::test();
	//sandbox::heap<double> h;
	//h.insert(0.2);

	//heap::test();
	//sorting::test();
	DP::test();
	system("pause");
	return 0;
}

