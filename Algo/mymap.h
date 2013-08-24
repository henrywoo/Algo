#ifndef __ALGO_MYMAP__
#define __ALGO_MYMAP__

#include <map>

namespace mymap{

    ///@brief bimap
    void test(){
        map<string,int> m1;
        m1[string("a")] = 10;
        m1[string("b")] = 9;
        m1[string("c")] = 8;
        m1[string("d")] = 7;
        m1[string("e")] = 6;
        m1[string("f")] = 5;
        m1[string("g")] = 7;
        
        multimap<int,string> m2;
        m2.insert(make_pair(10,"a"));
        m2.insert(make_pair(9,"b"));
        m2.insert(pair<int,string>(8,"c"));
        m2.insert(pair<int,string>(7,"d"));
        m2.insert(pair<int,string>(6,"e"));
        m2.insert(pair<int,string>(5,"f"));
        m2.insert(pair<int,string>(7,"g"));

        for(multimap<int,string>::reverse_iterator i=m2.rbegin();
            i!=m2.rend();
            ++i)
        {
            cout << i->second << "\t" << i->first << endl;
        }
    }

};

#endif