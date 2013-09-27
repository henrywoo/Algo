#include <list>
#include <unordered_set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>


namespace DisjointSet{
    using namespace std;

#if 0
    template<class T1,class T2,class T3>
    T1<T2<T3>> ComputeDisjointSets_(const T1<T2<T3>>& vec_si){
#define TSI T2<T3>
#define VSI T1<TSI>
#define MSS map<string,TSI>
        if (vec_si.empty()){
            return VSI();
        }else{
            int sz=vec_si.size();
            if (sz==1){
                return vec_si;
            }else{
                TSI tmp;
                // O(n) - n # of elements
                for (VSI::const_iterator i=vec_si.begin();i!=vec_si.end();++i){
                    const TSI& si=*i;
                    for (TSI::iterator j=si.begin();j!=si.end();++j){
                        tmp.insert(*j);
                    }
                }

                map<string,TSI> mss;
                for (TSI::iterator j=tmp.begin();j!=tmp.end();++j){
                    const int& t=*j;
                    string s;
                    int counter=0;
                    for (VSI::const_iterator i=vec_si.begin();i!=vec_si.end();++i){
                        counter++;
                        const TSI& si=*i;
                        if (si.find(t)!=si.end()){
                            s += (char)(counter+'0');
                        }
                    }
                    if (mss.find(s)==mss.end()){
                        TSI ns;
                        ns.insert(t);
                        mss[s] = ns;
                    }else{
                        TSI& si = mss[s];
                        si.insert(t);
                    }
                }

                VSI r;
                for (MSS::iterator i=mss.begin();i!=mss.end();++i){
                    TSI si=i->second;
                    r.push_back(si);
                }
                return r;
            }
        }
    }
#endif

#define TSI unordered_set<int>
#define VSI vector<TSI>
#define MSS map<string,TSI>

    VSI ComputeDisjointSets(const VSI& vec_si){
        if (vec_si.empty()){
            return VSI();
        }else{
            int sz=vec_si.size();
            if (sz==1){
                return vec_si;
            }else{
                TSI tmp;
                // O(n) - n # of elements
                for (VSI::const_iterator i=vec_si.begin();i!=vec_si.end();++i){
                    const TSI& si=*i;
                    for (TSI::iterator j=si.begin();j!=si.end();++j){
                        tmp.insert(*j);
                    }
                }

                map<string,TSI> mss;
                for (TSI::iterator j=tmp.begin();j!=tmp.end();++j){
                    const int& t=*j;
                    string s;
                    int counter=0;
                    for (VSI::const_iterator i=vec_si.begin();i!=vec_si.end();++i){
                        counter++;
                        const TSI& si=*i;
                        if (si.find(t)!=si.end()){
                            s += (char)(counter+'0');
                        }
                    }
                    if (mss.find(s)==mss.end()){
                        TSI ns;
                        ns.insert(t);
                        mss[s] = ns;
                    }else{
                        TSI& si = mss[s];
                        si.insert(t);
                    }
                }

                VSI r;
                for (MSS::iterator i=mss.begin();i!=mss.end();++i){
                    TSI si=i->second;
                    r.push_back(si);
                }
                return r;
            }
        }

    }


    bool test(){
        int tmp1[]={ 1, 2, 3 };
        TSI s1(tmp1,tmp1+_countof(tmp1));
        int tmp2[]={ 2, 3, 4, 5 };
        TSI s2(tmp2,tmp2+_countof(tmp2));
        int tmp3[]={ 2, 3, 4, 5, 6 };
        TSI s3(tmp3,tmp3+_countof(tmp3));
        int tmp4[]={ 5 };
        TSI s4(tmp4,tmp4+_countof(tmp4));

        VSI vsi;
        vsi.push_back(s1);
        vsi.push_back(s2);
        vsi.push_back(s3);
        vsi.push_back(s4);

        VSI r=ComputeDisjointSets(vsi);
        for (VSI::iterator i=r.begin();i!=r.end();++i){
            TSI& si=*i;
            copy(si.begin(),si.end(),ostream_iterator<int>(cout," "));
            cout<<endl;
        }
        return true;
    }

}

