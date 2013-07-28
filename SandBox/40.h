#pragma once
#include <iostream> 
using namespace std;
namespace _40{

#define SSS "sdf" ## "sdfs"
#define SSS2 "sdf"  "sdf\
s"
#define SSS3(x) #x

	void test(){
		cout<< SSS << endl;
		cout<< SSS2 << endl;
		cout<< SSS3(asv) << endl;
#if 1
		int *p3=new int[100];
		cout << hex << p3 << endl;
		delete [] p3;

		int *p2=new int[20];
		cout << hex << p2 << endl;
		delete [] p2;

		int* (*p)[5]=new int* [3][5];
		cout << hex << p << endl;
		delete [] p;
		
		//int *p2=new int[100];
		//cout << hex << p2 << endl;

		int* (*p_)[5]=new int* [6][5];
		cout << hex << p_ << endl;
		delete [] p_;
		


#else
		
#endif
	}


}