#pragma once
#include <iostream> 
using namespace std;

namespace _38{


class Object 
{ 
public: 
	Object() {} 

	void Print() const 
	{ 
		cout << "const" << endl; 
	} 

	void Print() 
	{ 
		cout << "mutable" << endl; 
	}
};

void print_obj(const Object& obj) 
{ 
	obj.Print(); 
} 

int test() 
{ 
	Object       obj1; 
	const Object obj2; 
	Object*const pobj1 = &obj1; 

	print_obj(obj1); 
	print_obj(obj2); 

	obj1.Print(); 
	obj2.Print(); 

	pobj1->Print(); 

	exit(0);
	return 0; 
}

}


