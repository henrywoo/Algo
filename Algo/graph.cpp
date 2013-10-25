#include "graph.h"

namespace graph{
    using namespace std;

    typedef vector<pair<gnode*,int>> adjVec;

    struct gnode{
        union{
            int index;
            char* name;
        };
        
        adjVec v;
        int minlength;

        gnode(char* s):name(s),minlength(INT_MAX){}
    };

    struct lengthcompare{
        bool operator()(const gnode& x, const gnode& y) const{
            return x.minlength > y.minlength;
        }
    };

    typedef priority_queue<gnode, vector<gnode>, lengthcompare> myPQ;

    void test(){
        //vertex
        gnode gs("s");
        gnode gt("t");
        gnode gx("x");
        gnode gy("y");
        gnode gz("z");

        //edges
        gs.v.push_back(pair<gnode*,int>(&gt,10));
        gs.v.push_back(pair<gnode*,int>(&gy,5));

        gt.v.push_back(pair<gnode*,int>(&gx,1));
        gt.v.push_back(pair<gnode*,int>(&gy,2));

        gx.v.push_back(pair<gnode*,int>(&gz,4));

        gz.v.push_back(pair<gnode*,int>(&gx,6));
        gz.v.push_back(pair<gnode*,int>(&gs,7));

        gy.v.push_back(pair<gnode*,int>(&gx,9));
        gy.v.push_back(pair<gnode*,int>(&gt,3));
        gy.v.push_back(pair<gnode*,int>(&gz,2));

        //
        gs.minlength=0; // source

        myPQ pq;
        pq.push(gs);
        pq.push(gt);
        pq.push(gx);
        pq.push(gz);
        pq.push(gy);

        // open a thread to 

        while (!pq.empty()){
            gnode& g= pq.top();
            for (adjVec::iterator i=g.v.begin();i!=g.v.end();++i){
                pair<gnode*,int>& p=*i;
                if (p.first->minlength > p.second){
                    p.first->minlength = p.second;//???
                }
            }
            pq.pop();
            //make_heap(&pq.top(), &pq.top() + pq.size()*sizeof(pq.top()) + 1 );
        }
        
        
        //stack<int> s;g


        


    }
}
