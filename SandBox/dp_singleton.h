#ifndef __DP_SIGLETON__
#define __DP_SIGLETON__

class singleton{
private:
    singleton(){cout << "init singleton" << endl;}
    ~singleton(){}
    //singleton* p;

public:
    static singleton& R(){
        static singleton s;
        return s;
    }
    void Say(){cout<<"hello\n";}
};

class mtsingleton
{
private:
    mtsingleton();
    ~mtsingleton();
public:

};



#endif