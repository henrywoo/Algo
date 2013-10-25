#include <thread>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>

namespace mpc{

  using namespace std;

  //最近收到的100个请求，设计数据结构，不能用STL
  //////////////////////////////////////////////////
  template<class T>
  class node{
    T* p;
    node* next;
    node* prev;
  };
  class Req{
    //todo
  };
  template <int N,class T>
  class LastNBuf{//本质上是一个fixed length ListQueue
  public:
    enum bszie class { BUFSIZE = N };
    node<T>* sentinel;//benefit is: both push and pop are O(1)
    mutex mu;
    int cur_size;//current size of list

    LastNBuf():cur_size(0){
      sentinel = new node<T>;
    }
    
    bool push(node<T>* p){
      lock_guard<mutex> lg;
      if (cur_size == bszie::BUFSIZE){
        return false;
      }
      node<T>* n = sentinel->prev;
      sentinel->prev = p;
      
      p->next = sentinel;
      p->prev = n;

      n->next = p;
      ++cur_size;
      return true;
    } // push_back

    node<T>* pop(){
      lock_guard<mutex> lg;
      if (cur_size==0){
        return nullptr;
      }else{
        node<T>* n = sentinel->next;
        sentinel->next = n->next;
        n->next->prev = sentinel;
        --cur_size;
        return n;
      }
    } // pop_front
  };

  LastNBuf<100,Req> last100req;
  //////////////////////////////////////////////////

  queue<int> Q;
  int qsize = 0;
  const int MAXQSIZE = 5;

  mutex m;
  condition_variable cv;

  void producer(int i){
    unique_lock<mutex> lg(m);
    while (Q.size() == MAXQSIZE){
      cv.wait(lg);
    }
    //cv.wait(lg, []{return Q.size() < MAXQSIZE; });
    Q.push(i);
    if (Q.size() == 1)//有一个食物了
      cv.notify_all();
  }

  void consumer(){
    unique_lock<mutex> lg(m);
    //cv.wait(lg, []{return !Q.empty(); });
    while (Q.empty()){
      cv.wait(lg);
    }
    int i = Q.front();
    cout << i << endl;
    Q.pop();
    if (Q.size() == MAXQSIZE - 1)//有一个空位置了
      cv.notify_all();
  }

  void test(){
    vector<thread> threads;
    for (int i = 0; i < 50; ++i){
      threads.push_back(thread(producer,i));
    }
    for (int i = 50; i < 100; ++i){
      threads.push_back(thread(consumer));
    }
    for (thread& t: threads){
      t.join();
    }

    

  }

}

