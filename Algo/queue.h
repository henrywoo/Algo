#include <atomic>
#include <list>

using namespace std;

namespace mtqueue{
    // single thread
    class queue_s{
        list<int> li;
        queue_s(){}
        void enqueue(int a){
            li.push_front(a);
        }
        void dequeue(){
            li.pop_back();
        }
    };

    class node{
        int i;
        node* next;
    };

    class queue_mt{
        node* sentinel;
        CRITICAL_SECTION h;
        CRITICAL_SECTION t;
        node* ptail;
        queue_mt(){
            sentinel=new node;
            sentinel->i=INT_MAX;
            sentinel->next=NULL;
            ptail=sentinel;
        }

        //enqueue has no impact on ptail
        void enqueue(int i){
            node* tmp=new node;
            tmp->i=i;
            //
            if(sentinel->next==NULL){
                ptail=tmp;
            }
            tmp->next=ptail;// need to lock sentinel's next node
            //
        }

        //
        void dequeue(){

        }


    };

}
