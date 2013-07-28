// SandBox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>




using namespace std;
#include "38.h"
#include "39.h"

class ExA{
private:
	int _x1;
	char _x2;
	static int stat;
	friend int Friend0(){return ExA::stat;}
public:
	//ExA(){}
	ExA(const ExA&){}
	static int GetStat(){return stat;}
};
int ExA::stat=1;

class ExB1:public virtual ExA{
	//friend int Friend1(){return ExA::stat;}
	//sandbox.cpp(19): error C2248: 'ExA::stat' : 
	//cannot access private member declared in class 'ExA'
	//friend int Friend1(){return ExB1::ExA::stat;}
};

class ExB2: public virtual ExA{};

class ExC:public ExB1, public ExB2{};

struct AA{int i;} aa;
typedef struct BB{int i;} bb;

const bb bbbb={1};
void f(const bb& b){}

class simon{
public:
	simon(){}
	simon(simon&){}
};

//const int MON=0;
enum DOW{MON=0};

template<class T>
class Base{};

template<class T, class Q>
class Derived: Base<T>{};


struct Base1{

	Base1(){cout << "Base1" <<endl;}
	struct N{
		N(){cout << "N" <<endl;}
	};
};
struct Derived1:public Base1{
	Derived1(){cout << "Derived1" <<endl;}
};

struct person{
	virtual void name()=0;
};
struct son{
	virtual void name(){cout << "Jack" << endl;}
	virtual void hello(){cout << "hh" << endl;}
};
struct dad:public son{
	dad(){cout << "i am daddy" << endl;}
	void name(){cout << "simon" << endl;}
	void hello(){cout << "bb" << endl;}
};


void obj2(int b=22){
	int a=1;
	cout << hex << &a << endl;
	cout << hex << &b << endl;
}

class human{
protected:
	int age;
public:
	human():age(10){}
	virtual void speak(const char* s){cout<< "I speak:" << s << age << endl;}
};
class parrot1:private human{
	
public:
	//virtual void speak(const char* s){cout<< "I speak: yoyo is " << this->age  << endl;}
	void fly(){cout<< "I fly." << endl;};
	void perform(){
		speak("hello");
		fly();
	}
};

#include "40.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int uuu=0.5;
	//_38::test();
	//_39::test();
	_40::test();
	parrot1 p1;
	p1.perform();
	cout << hex << &_tmain << endl;
	cout << hex << &obj2 << endl;
	obj2();
	int localint=0;
	cout << hex << &localint << endl;
	cout << hex << &argc << endl;
	cout << hex << &argv << endl;
	dad sim;
	cout << hex << &sim << endl;
	//cout << hex << &(dad::*name) << endl;
	sim.name();
	cout << hex << &(obj2) << endl;

	dad* pdad=new dad[20];
	obj2();
	cout << hex << &pdad << endl;
	cout << hex << pdad << endl;
	delete [] pdad;

	typedef void(*fun)();
	int* address_of_simon=reinterpret_cast<int*>(&sim);
	int* address_of_vtbl=reinterpret_cast<int*>(&address_of_simon);
	fun F=reinterpret_cast<fun>(*(address_of_vtbl+1));
	fun pFun = (fun)*((int*) * (int*)(&sim) );
	pFun();
	F();

	Derived1 d1;
	int var=DOW::MON;
	DOW x=static_cast<DOW>(MON+1);
	int var2=DOW::MON;
	char r='\0';
	int y=2;
	cout << "Address of var:" << hex << &var << endl;
	cout << "size of var:" << hex << sizeof(var) << endl;
	cout << "Address of y:" << hex << &y << endl;//004FF9A4
	
	simon sm;
	bb bbb;
	bbb.i;
	aa.i;
	f(bbbb);
	std::cout << "int is: " << typeid(sm).name() << '\n';
	cout << sizeof(ExA) << endl;
	cout << sizeof(ExB1) << endl;
	cout << sizeof(ExB2) << endl;
	cout << sizeof(ExC) << endl;

	/*ExA a;
	ExB1 b;
	cout << hex << (int)(&a) << endl;
	cout << hex << (int)(&b) << endl;*/

	//sandbox.cpp(37): error C2385: ambiguous access of 'ExA'
	//cout << ExC::ExA::GetStat() << endl;

	return 0;
}

