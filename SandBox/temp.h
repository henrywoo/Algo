// template programming header
// meta programming
template<int N>
struct Fibonacci{
    enum{c=Fibonacci<N-1>::c + Fibonacci<N-2>::c};
};

// complete specialization
template<>
struct Fibonacci<1>{
    enum{c=1};
};

template<>
struct Fibonacci<2>{
    enum{c=2};
};


template <class T>
struct StaticVarStruct{
    static int var;
};

template<class T>
int StaticVarStruct<T>::var = 10;