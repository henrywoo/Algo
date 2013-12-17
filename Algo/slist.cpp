#include "list.h"
#include <iostream>

slist::slist() :head(NULL){}

slist::slist(int* p, int* q) : head(NULL){
  while (p != q){
    push(*p++);
  }
}
slist::slist(int* p, int size) : head(NULL){
  while (size-- >= 0){
    push(*p++);
  }
}
slist::slist(const vector<int>& v){}

slist::~slist(){
  slist_node* tmp = NULL;
  while (head->next != NULL){
    tmp = head;
    head = head->next;
    delete tmp;
  }
}

// the original head was pushed to the rear
void slist::push(int n){
  if (!head){
    head = new slist_node(n);
    head->next = NULL;
  }
  else{
    slist_node* tmp = head;
    head = new slist_node(n);
    head->next = tmp;
  }
  this->lsize++;
}

void slist::push(int* p, int* q){}

slist_node* slist::end(){
  //return tail->next;
  return NULL;
}

slist_node* slist::begin(){
  return head;
}

const slist_node* __cdecl slist::FindKthToTail(int k) const throw(){
  try{
    if (k < 0){
      return NULL;
    }
    else if (k == 0){/// just return the tailnode itself
      slist_node* tmp = head;
      slist_node* prevtmp = NULL;/// the prev of tmp
      while (tmp != NULL){
        prevtmp = tmp;
        tmp = tmp->next;
      }
      return prevtmp;
    }
    else{
      slist_node* tmp1 = head;
      slist_node* tmp2 = head;
      unsigned int count = 0;/// the size of the slist
      while (tmp1 != NULL){
        tmp1 = tmp1->next;
        if (count++ > k){
          tmp2 = tmp2->next;
        }
      }
      if (count < k + 1){/// no move for tmp2, which means K+1 > length of the link
        return NULL;
      }
      else{
        return tmp2;
      }
    }
  }
  catch (...){ return NULL; }
}

// swap node is equivalent to swap data
void SwapNode(slist_node* sn1, slist_node* sn2){
  ::swap(sn1->d, sn2->d);
}

slist_node* tailsnode(slist_node* h){
  slist_node* tmp = h;
  while (tmp->next){
    tmp = tmp->next;
  }
  return tmp;
}

slist_node* PartitionSList(slist_node* n1, slist_node* n2, COMP pred)
{
  slist_node* pivot = n1;
  int pdata = pivot->d;
  slist_node* pBBhead = nullptr, *pBBtail = nullptr; ////Error Prone
  slist_node* tmp = n1->next;
  slist_node* psmalltail = nullptr;
  while (tmp){
    /*if ((tmp->d) > pdata){*/
    if (pred(tmp->d, pdata)){
      if (pBBhead == pBBtail && pBBtail == nullptr){
        pBBhead = pBBtail = tmp;
      }
      else{
        pBBtail = tmp;
      }
    }
    else{
      if (pBBhead){
        SwapNode(pBBhead, tmp);
        psmalltail = pBBhead;
        pBBhead = pBBhead->next;
        pBBtail = tmp;
      }
      else{
        psmalltail = tmp;
      }
    }
    tmp = tmp->next;
  }
  if (psmalltail){
    SwapNode(pivot, psmalltail);
  }else{
    psmalltail = pivot;
  }
  return psmalltail;/// return the pivot
}

void qsortSList(slist_node* h, slist_node* t, COMP pred){
  if (h == NULL || t == NULL || h==t){ return; }
  slist_node* p = PartitionSList(h, t, pred);
  qsortSList(h, p, pred);
  if (p){
    qsortSList(p->next, t, pred);
  }
}

void SortSList(slist_node* sn, COMP pred){
  qsortSList(sn, tailsnode(sn), pred);
}

void slist::sort(COMP pred){
  SortSList(head,pred);
}

bool slist::test(){
  int a[] = { 10, 5, 7, 6, 40, 25, 50, 13, 21, 16, 19, 9, 23, 8 };
  slist sl(a, a+_countof(a));
  sl.printall_reversely();
  int j = (sizeof(a) / sizeof(int)-1);
  for (int i = -1; i <= j; ++i){// test case 1
    const slist_node* p = sl.FindKthToTail(i);
    cout << hex << p << "\t" << (p ? p->d : 0) << endl;
  }

#if 0
  for (slist_node* i = sl.begin(); i != sl.end(); i = i->next){
    cout << i->d << endl;
  }
#endif

  // The paradigm of loop a slist
  slist_node* i = sl.begin();
  slist_node* prevnode = NULL;// can be assigned to i, it doesnt matter
  while (i != NULL){
    prevnode = i;
    i = i->next;
  }
  cout << dec << "Last node:" << prevnode->d << endl;

  sl.sort();
  sl.printall();

  sl.sort();
  sl.printall();

  sl.sort([](int a, int b){return a < b; });
  sl.printall();
  
  std::sort(a, a + _countof(a), std::greater<int>());
  slist sl2(a, a + _countof(a));
  sl2.sort();
  sl2.printall();

  return true;
}

