#pragma once

namespace smartptr{

  /// reference count pointer
  template<typename T>
  class RCPtr{
    using uint = unsigned int;
  public:
    T* p=nullptr;
    uint* rc=nullptr;

    RCPtr(){
      cout << __FUNCSIG__ << "\t" << __LINE__ << endl;
      cout << "new obj" << endl;
    }

    ~RCPtr(){
      giveup();
      cout << __FUNCSIG__ << "\t" << __LINE__ << "\trc=" << (rc?(*rc):0) << endl;
    }

    void giveup(){
      if (rc){
        --(*rc);
        if (*rc == 0){
          delete p;
          delete rc;
          reset();
        }
      }
    }

    RCPtr(T* rp){// take ownership for the raw pointer
      cout << __FUNCSIG__ << "\t" << __LINE__ << endl;
      //giveup();
      p = rp;
      rc = new uint(1);
    }

    //copy ctor
    RCPtr(const RCPtr& x){
      cout << __FUNCSIG__ << "\t" << __LINE__ << endl;
      if (this == &x){ return; }
      p = x.getp();
      rc = x.getrc();
      if (rc){
        (*rc)++;
      }
    }

    //move ctor
    RCPtr(RCPtr&& x){
      cout << __FUNCSIG__ << "\t" << __LINE__ << endl;
      giveup();
      p = x.getp();
      rc = x.getrc();
      x.reset();
    
    }
    // move assignment
    RCPtr& operator=(RCPtr&& x){
      cout << __FUNCSIG__ << "\t" << __LINE__ << endl;
      giveup();
      p = x.getp();
      rc = x.getrc();
      x.reset();
      return *this;
    }

    void reset(){ p = nullptr; rc = nullptr; }

    // copy assignment
    RCPtr& operator=(const RCPtr& x){
      if (this == &x){ return *this; }
      giveup();
      p = x.getp();
      rc = x.getrc();
      if (rc){
        (*rc)++;
      }
      return *this;
    }

    T* operator->()const { return p; }
    T& operator*()const { return *p; }
    T* getp()const{ return p; }
    uint* getrc() const{ return rc; }
    operator void*(){ return p; }

  };

  struct metadata{
    size_t sz:32;
    metadata* next;
    size_t isfree;
  };

  void test(){
    {
      cout << sizeof (char*) << endl;
      cout << sizeof (metadata) << endl;
      vector<unique_ptr<int>> vui;
      unique_ptr<int> ui = make_unique<int>(5);
      unique_ptr<int> ui2{ move(ui) };
      //vui.push_back(ui2);
      vui.push_back(make_unique<int>(50));
    }

    RCPtr<int> x1(new int(1));
    RCPtr<int> x2(new int(2));
    RCPtr<int> x3(x2);
    RCPtr<int> x4(new int(4));
    x1 = move(x3);
    x4 = x2;
    RCPtr<string> x5(new string("hello"));
    if(x5)
      cout << *x5 << "\t" << x5->size() << endl;
    swap(x1, x4);
#if 1
    shared_ptr<int> si1(make_shared<int>(1));
    shared_ptr<int> si2(make_shared<int>(2));
    shared_ptr<int> si3{ si1 };
    vector<shared_ptr<int>> vsi;
    vsi.push_back(make_shared<int>(1));
    vsi.push_back(make_shared<int>(2));
    vector<shared_ptr<int>> vsi2 = vsi;
    for (int i = 0; i < vsi2.size(); ++i){
      cout << *vsi2[i] << endl;
    }
#endif
    try{
      vector<RCPtr<int>> vi;
      vi.push_back((new int(1)));
      vi.push_back((new int(2)));
      vector<RCPtr<int>> vi2 = vi;
      for (int i = 0; i < vi2.size(); ++i){
        cout << *vi2[i] << endl;
      }
    }catch (exception& e){
      cout << e.what() << endl;
    }
  }
}
