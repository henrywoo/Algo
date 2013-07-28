#pragma once
#include <iostream> 
using namespace std;
namespace _39{

	/// const - your program should not change it
	/// volatile - other program might change it, so don't over-optimize it

	class TestPrint 
	{ 
	public:
		void Print() 
		{ 
			std::cout << "TestPrint" << std::endl; 
		} 

		void Print() const 
		{ 
			std::cout << "const TestPrint" << std::endl; 
		} 

		void Print() volatile 
		{ 
			std::cout << "volatile TestPrint" << std::endl; 
		} 

		void Print() const volatile 
		{ 
			std::cout << "const volatile TestPrint" << std::endl; 
		} 
	}; 


	void test() 
	{ 
		TestPrint normal_test; 
		normal_test.Print(); 

		const TestPrint const_test; 
		const_test.Print(); 

		volatile TestPrint volatile_test; 
		volatile_test.Print(); 

		const volatile TestPrint const_volatile_test; 
		const_volatile_test.Print(); 
	}
}