#ifndef _ALGO_UTIL_
#define _ALGO_UTIL_
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<string> split(const string& str_,char delim);

double getmean(double *p, size_t sz);

double maxsubarray_fixed(double *p, size_t sz, double*& end);

double maxsubarray_fixed_reverse(double *p, size_t sz, double*& end);


///@brief Largest Sum Contiguous Subarray
///@param
/// start - pointer to start of max subarray
/// end   - pointer to end of max subarray
double maxsubarray(double *p, size_t sz, double*& start, double*& end);

namespace util{
	void test();

	void swap(int& a,int& b);
	template<typename T>
	void swap(T& x,T& y){
		if (x!=y)
		{
			x^=y;
			y^=x;
			x^=y;
		}
	}
}




template<typename T>// for shared_ptr
struct array_deleter{
	void operator()(T* p){delete [] p;}
};


#endif // _ALGO_UTIL_