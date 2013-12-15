#pragma once
#include <iostream> 
using namespace std;

namespace _38{

class Object {
public: 
	Object() {} 

	char Print() const{
		cout << "const" << endl;
        return 0;
	}
	int Print(){
		cout << "mutable" << endl; 
        return 0;
	}
    //double Print(){return 0.0;}//不能仅凭返回值来区别函数,但是可以凭是否是const
};

void print_obj(const Object& obj){
	obj.Print();
} 

int test(){ 
	Object       obj1; 
	const Object obj2; 
	Object* const pobj1 = &obj1; // the pointer itself is not modifiable
    Object const *pobj2 = &obj1; // the contents pointed by the pointer cannot be modified

	print_obj(obj1); 
	print_obj(obj2);

	obj1.Print(); 
	obj2.Print(); 

	pobj1->Print();

	return 0; 
}

}


