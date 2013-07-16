#ifndef _ALGO_UTIL_
#define _ALGO_UTIL_
#pragma once

#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> split(const string& str_,char delim){
	string str(str_);
	vector<string> v;
	int pos=str.find(delim);
	if (pos==string::npos){
		v.push_back(str);
		return v;
	}
	while(pos!=string::npos){
		string str1=str.substr(0,pos);
		v.push_back(str1);
		str=str.substr(pos+1,str.size()-pos-1);
		pos=str.find(delim);
	}
	v.push_back(str);
	return v;
}

// template<typename T>
// void swap_(T& x, T& y){
// 	T tmp;
// 	tmp=x;
// 	x=y;
// 	y=tmp;
// }





#endif // _ALGO_UTIL_