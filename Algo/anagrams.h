#ifndef __ALGO_ANAGRAMS__
#define __ALGO_ANAGRAMS__

#include <algorithm> //sort

/*
Given an array of strings, return a list of all groups of 
strings that are anagrams.
Note: All inputs will be in lower-case.

For example, ["tea","and","ace","ad","eat","dan"] 
=> ["and", "dan", "tea", "eat"]
*/

namespace anagrams{
    ///This algorithm runs in time O(n) and uses O(n) space
    vector<string> GetAnagrams(const vector<string> &vs) {
#define PROCESS1stITEM -1
        vector<string> r;
        map<string, int> anagram;
        for (int i = 0; i < vs.size(); ++i){
            string s = vs[i];
            sort(s.begin(), s.end());//??? NlogN
            if (anagram.find(s) == anagram.end()){
                anagram[s] = i;
            }else{
                // coz we need to add the first one
                if (anagram[s] != PROCESS1stITEM) {
                    r.push_back(vs[anagram[s]]);
                    anagram[s] = PROCESS1stITEM;
                    r.push_back(vs[i]);
                }else{
                    // we have added the first item
                    r.push_back(vs[i]);
                }
            }
        }
        return r;
    }


    void test(){
        const char *ca[]=
        {"tea","and","ace","ad","eat","dan","ate"};
        vector<string> vs(ca,ca+_countof(ca));
        vs=anagrams::GetAnagrams(vs);
        for (vector<string>::const_iterator i=vs.begin();i!=vs.end();++i){
            cout << i->c_str() << endl;
        }
    }
}






#endif

