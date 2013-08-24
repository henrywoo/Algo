#ifndef __ALGO_WORDLADDER__
#define __ALGO_WORDLADDER__

#include <unordered_set>

using namespace std;

/// log dot
bool isoneeditaway(const string& s1, const string& s2)
{
    if (s1.size()!=s2.size()){
        return false;
    }else{
        int count=0;
        for (int i=0;i<=s1.size()-1;++i){
            if (s1[i]!=s2[i]){
                ++count;
                if (count==2){
                    return false;
                }
            }
        }
        return count==1;
    }
}

#define OneEditWordsVec vector<vector<string>>
//dict [in]  - the dictionary as a set such as [hot, dot, lot, dot, dog, log, cog]
//oewn [out] - 2 D one edit word away array
//<hot dot lot>
//<dot dog>
//<lot log>
//<dog log cog>
void GetOneEditWords(const set<string>& dic,OneEditWordsVec& oewv){
    ///@todo
}


#endif