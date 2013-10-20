#include <iostream>

namespace shl{
    using namespace std;

    struct Base{
        Base(){
            cout << "Base" << endl;
        }
    };

    struct Sub1: public virtual Base{};
    struct Sub2: public Base{};
    struct Multi: public Sub1, public Sub2{};

    struct A{
        static int counter;
        A(int value):m_value(value){}
        A(const A& a){
            m_value=a.m_value;
            counter++;
            cout<< counter << endl;
        }
        int m_value;
    };
    int A::counter=0;

    struct B:A{
        B(int value):
            A(value){}
    };

    class Foo{
    public:
        Foo(){cout<<"Foo ";}
        virtual ~Foo(){}
    };
    class Bar: virtual public Foo{
    public:
        Bar(){cout<<"Bar ";}
    };
    class FooToo: virtual public Foo{
    public:
        FooToo(){cout<<"FooToo ";}
    };
    class FooTooBar: virtual public FooToo, public Bar{
    public:
        FooTooBar(){cout<<"FooTooBar ";}
    };

    namespace AAA{
        void f(){}
    }

    namespace BBB{
        namespace{int x;}
        void f(int){}
    }

    class FFF{
    public:
        FFF(int x){}
        void f(int);
    };
    void (FFF::*fFFFF)(int);
    
    using namespace BBB;
    
    template<class T, class C> void foo(C x,T y){};
    template<class T1, class T2>
    class Obj{
        T1 t1;
        T2 t2;
    public:
        Obj(T1 t1_,T2 t2_):t1(t1_),t2(t2_){}
        operator const T1(){return t1;}
    };

    int test_shl(){
        

        int* p=new (std::nothrow) int[1<<9];
        if (!p){
            cout << "No memory\n";
        }
        int i;
        Obj<int,float> o(15,10.375);
        i=o;

        Foo& foo=dynamic_cast<Foo&>(*new FooTooBar);
        FFF f('a');
        x;

        string s;
        //if ("sds"==s){}
        FooTooBar vvv;
        try{
            try{
                B b(5);
                throw b;
            }catch(A a){
                a.m_value *= 2;
                throw;
                //The throw-expression without an operand may only be used inside a catch block (it calls std::terminate if used otherwise). 
                //It abandons the execution of the catch block and passes control to the next matching catch clause up the call stack (but 
                //not to another catch clause after the same try block), reusing the existing exception object: no new objects are made. 
            }catch(B b){
                b.m_value -= 2;
                throw b;
            }
        }catch (A a){
            std::cout << endl <<
                a.m_value << endl;
        }
        return 0;
    }

}
