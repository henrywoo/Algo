#include <functional>
#include <string>
#include <iostream>

namespace std_bind{
  using namespace std;
  using namespace std::placeholders;

  void show(const string& a, const string& b, const string& c){
    cout << a << "; " << b << "; " << c << endl;
  }

  void test(){
    auto x = bind(show, _1, _2, _3);
    auto y = bind(show, _3, _1, _2);
    auto z = bind(show, "hello", _2, _1);

    x("one", "two", "three");
    y("one", "two", "three");
    z("one", "two");

  }
}
