#ifndef __DP_OBSERVER__
#define __DP_OBSERVER__

#include <unordered_set>

namespace designpattern{

    using namespace std;
    class observer{
    public:
        virtual void update(void*);
        virtual ~observer(){}
    };

    class A:public observer{

    };


    class movingtarget{
        unordered_set<observer*> ob;
    public:
        void addob(observer* x){ob.insert(x);}
        void moveto(int pos){
            //1. move

            //2. notify
            for (unordered_set<observer*>::iterator i=ob.begin();i!=ob.end();++i){
                (*i)->update(&pos);
            }
        }
    };


}

#endif