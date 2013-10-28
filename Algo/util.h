#ifndef _ALGO_UTIL_
#define _ALGO_UTIL_
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#define mymax(a,b) (a>b?a:b)
#define mymin(a,b) (a>b?b:a)

vector<string> split(const string& str_,char delim);

///@brief To replace all oldstr inside str with newstr
///@param
/// str - original string\n
/// oldstr - string to be replaced
/// newstr - string to be inserted into the original string
///@return
/// A new string with all oldstr replaced by newstr
///@test
/// StringReplace("","xyz","abc")   return string("")
/// StringReplace("opq","","abc")   return string("opq")
/// StringReplace("opq","abc","")   return string("opq")
/// StringReplace("opq","o","")     return string("pq")
/// StringReplace("opoq","o","abc") return string("abcpabcq")
string StringReplace(const string& str, const string& oldstr, const string& newstr);

void removeWhiteSpace(char* p,size_t sz);

double getmean(double *p, size_t sz);

double maxsubarray_fixed(double *p, size_t sz, double*& end);

double maxsubarray_fixed_reverse(double *p, size_t sz, double*& end);

int power(unsigned int base, unsigned int exponent);
bool testpower();

unsigned Power(unsigned n, unsigned p);

///@brief Largest Sum Contiguous Subarray
///@param
/// start - pointer to start of mymax subarray
/// end   - pointer to end of mymax subarray
double maxsubarray(double *p, size_t sz, double*& start, double*& end);
double maxsubarray(double *head, double *tail);
/// replace % with %%
string processpercentsign(const string& sss);

namespace util{
	void test();

	void swap(int& a,int& b);
	template<typename T>
	void swap(T& x,T& y){
		if (x!=y){
			x^=y;
			y^=x;
			x^=y;
		}
	}

    template<typename T>
    void swap(T* x,T* y){
        if (x!=y){
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


#if 0
/// @brief a functor of making wstring like this:
/// @brief wstring hello = wstringmaker() << "hello" << 123;
/// @brief fail - empty wstring; succeed - a non-empty wstring
class wstringmaker{
private:
    wstring str_;
    unsigned int codepage_;
public:
    wstringmaker(unsigned int cp=CP_UTF8):codepage_(cp){}
    wstringmaker(const wstring& a):codepage_(CP_UTF8),str_(a){}
    wstringmaker(const wstring& a,unsigned int cp):codepage_(cp),str_(a){}
    wstringmaker& operator<<(unsigned long dwNum);
    wstringmaker& operator<<(const wstring& stradd);
    wstringmaker& operator<<(const string& stradd);
    operator wstring() const{return str_;}
    const wchar_t* c_str() const{return str_.c_str();}
};

class stringmaker{
private:
    string str_;
    unsigned int codepage_;
public:
    stringmaker(unsigned int cp=CP_UTF8):codepage_(cp){}
    stringmaker(const string& a):codepage_(CP_UTF8),str_(a){}
    stringmaker(const string& a,unsigned int cp):codepage_(cp),str_(a){}
    stringmaker& operator<<(unsigned long dwNum);
    stringmaker& operator<<(const string& stradd);
    stringmaker& operator<<(const wstring& stradd);
    operator string() const{return str_;}
    const char* c_str() const{return str_.c_str();}
};
#endif

#endif // _ALGO_UTIL_