#ifndef __ALGO_THREESUM__
#define __ALGO_THREESUM__
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
// set of set 会报错
// The C++ Standard does not provide a hash for this type.
// Hashing User-Defined Types in C++1y
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3333.html#helping.users

namespace algo{

    using namespace std;
    namespace threesum{

        typedef set<vector<int>> SVI;
        SVI getthreesum(vector<int> &vi, int target=0){
            if (vi.size()<3){
                return SVI();
            }
            //1 sort the array - O(NlogN)
            sort(vi.begin(),vi.end());
            SVI r;
            int sz=vi.size();
            vector<int> v(3);//3 is the size
            for (int i=0;i<sz-2;++i){
                int j=i+1;
                int k=sz-1;
                // [0]...[i][j]->...<-[k]
                while (j < k){ // better than j!=k
                    int tmp=vi[i]+vi[j]+vi[k];
                    if (tmp==target){
                        v[0]=vi[i];
                        v[1]=vi[j];
                        v[2]=vi[k];
                        r.insert(v);
                        ++j;//or --k; ///Crux
                    }else if (tmp>target){
                        k--;
                    }else{
                        j++;
                    }
                }
            }
#if 0 // error
            /*
            input	    output	                expected	
            [1,2,-2,-1]	[[-2,1,1],[-1,-1,2]]	[]
            */
            //2 put the array into a hash
            for(vector<int>::iterator i=vi.begin();i!=vi.end();++i){
                us.insert(*i);
            }
            //3 check if -(vi[1]+vi[2]) is in the hash O(N^2)
            VVIN r; // O(N^2)
            for (int i = 0; i < vi.size() -1; i++){
                for (int j=i+1;j<vi.size();++j){
                    if (us.find(-vi[i]-vi[j])!=us.end()){
                        ///tricky
                        vector<int> tmp;
                        tmp.push_back(vi[i]);
                        tmp.push_back(vi[j]);
                        tmp.push_back(-vi[i]-vi[j]);
                        sort(tmp.begin(),tmp.end());
                        if (find(r.begin(),r.end(),tmp)==r.end()){
                            r.push_back(tmp);
                        }
                    }
                }
            }
#endif
            return r;
        }

        int threeSumClosest(vector<int> &vi, int target) {
            int sz=vi.size();
            if (sz<3){
                return -1;
            }
            sort(vi.begin(),vi.end());
            int r=INT_MAX;
            int d=INT_MAX;
            for (int i=0;i<sz-2;++i){
                int j=i+1;
                int k=sz-1;
                while (j<k){
                    int tmp=vi[i]+vi[j]+vi[k];
                    if (tmp==target){
                        return tmp;
                    }else if(tmp>target){//// not tmp>0
                        if (d>abs(tmp-target)){
                            d=abs(tmp-target);
                            r=tmp;
                        }
                        --k;
                    }else{
                        if (d>abs(tmp-target)){
                            d=abs(tmp-target);
                            r=tmp;
                        }
                        ++j;
                    }
                }
            }
            return r;
        }

        void test(){
            int a[]={4,0,5,-5,3,3,0,-4,-5};//
            vector<int> tmp(a,a+_countof(a));
            SVI r=getthreesum(tmp);
            for (SVI::iterator i=r.begin();i!=r.end();++i){
                copy(i->begin(),i->end(),ostream_iterator<int>(cout," "));
                cout << endl;
            }
            int b[]={4,0,5,-5,3,3,0,-4,-5};
            vector<int> tmp2(b,b+_countof(b));
            cout << threeSumClosest(tmp2,-2) << endl;
        }
    }
    


}


#endif