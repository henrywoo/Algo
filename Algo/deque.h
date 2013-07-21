#ifndef __ALGO_DEQUE__
#define __ALGO_DEQUE__

#include <deque>
using namespace std;

void maxSlidingWindow(int A[], int n, int w, int B[]) {
	deque<int> Q;
	for (int i = 0; i < w; i++) {
		while (!Q.empty() && A[i] >= A[Q.back()])
			Q.pop_back();
		Q.push_back(i);
	}
	for (int i = w; i < n; i++) {
		B[i-w] = A[Q.front()];
		while (!Q.empty() && A[i] >= A[Q.back()])
			Q.pop_back();
		while (!Q.empty() && Q.front() <= i-w)
			Q.pop_front();
		Q.push_back(i);
	}
	B[n-w] = A[Q.front()];
}

namespace deque2{
	void test(){

		int a[]={1,  3,  -1, -3,  5,  3,  6,  7};
		int b[6]={0};
		maxSlidingWindow(a,sizeof(a)/sizeof(int),3,b);
	}
}

#endif