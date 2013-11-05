#include <memory>

namespace _list{

  using namespace std;

  template<typename T>
  struct node{
    T data;
    node* prev;
    node* next;
  };

  // a double circular linked list
  template <typename T>
  class dclist{
    shared_ptr<node<T>> psentinel;
  public:
    dclist(){
      psentinel = make_shared<node<T>>(*(new node<T>));
      psentinel->prev = psentinel->next = psentinel.get();
    }

    virtual ~dclist(){
    }
  };


  template <class T>
  void _slist_reverse(node<T>* head){
    // three pointers used!
    node<T>* prev = nullptr;
    node<T>* currenthead = head;
    while (currenthead){
      node<T>* tmp = currenthead;
      // update three variables!
      currenthead = tmp->next;
      tmp->next = prev;
      prev = tmp;
    }
  }

  template <class T>
  void _slist_printreversely(node<T>* head){
    if (head){
      _slist_printreversely(head->next);
      cout << head->data << endl;
    }
  }

  class KBeyondListSize{};
  class WrongParam{};

  template <class T>
  T _slist_FindKthToTail(node<T>* head, int k)
    throw(KBeyondListSize, WrongParam)
  {
    if ((k < 0) || (head==nullptr)) throw WrongParam();
    node<T> *t1 = head, *t2 = t1;
    for (int i = 0; i < k;++i){
      if (t2){
        t2 = t2->next;
      }else{
        throw KBeyondListSize();
      }
    }
    while (t2){
      t2 = t2->next;
      t1 = t1->next;
    }
    return t1->data;
  }



  void test(){
    node<int> n1{ 1, nullptr, nullptr };
    node<int> n2{ 2, nullptr, &n1 };
    node<int> n3{ 3, nullptr, &n2 };

    auto f = [](node<int>* variant){
      while (variant != nullptr){
        cout << variant->data << " ";
        variant = variant->next;
      }
      cout << endl;
    };

    try{
      _slist_FindKthToTail<int>(&n3, 0);
    }catch (KBeyondListSize&){
      cout << "Error" << endl;
    }
    
    f(&n3);
    _slist_printreversely(&n3);
    _slist_reverse(&n3);
    f(&n1);
  }

}