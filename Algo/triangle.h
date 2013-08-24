
#ifndef __ALGO_TRIANGLE__
#define __ALGO_TRIANGLE__

#include <vector>

#define VVINT vector<vector<int>>

namespace triangle{

    using namespace std;

/*
Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.

For example, given the following triangle

[
[2],
[3,4],
[6,5,7],
[4,1,8,3]
]

    2
   0 4
  6 5 0
-1 1 4 3

The minimum path sum from top to bottom is 11 (i.e., 2 + 3 + 5 + 1 = 11).

Note:
Bonus point if you are able to do this using only O(n) extra space, where n is the total number of rows in the triangle.
*/

    int minimumTotal(const VVINT &triangle) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int sz=triangle.size();
        VVINT vv(triangle);
        for (int i=sz-2;i>=0;--i){
            for (int j=0;j<=i;++j){
                vv[i][j] = vv[i][j]+min(vv[i+1][j],vv[i+1][j+1]);
            }
        }
        return vv[0][0];
    }


    

    vector<int> getNextRowInPascalTriangle(vector<int>& v)
    {
        vector<int> r;
        r.push_back(1);
        int sz=v.size();
        for (int i=0;i<sz-1;++i){
            r.push_back(v[i]+v[i+1]);
        }
        r.push_back(1);
        return r;
    }

    ///@brief n is the # of rows in pascal triangle
    VVINT generatePT(int n){
        if (n<1){
            return VVINT();
        }else{
            VVINT vv;
            vector<int> tmp(1,1);
            vv.push_back(tmp);
            if (n==1){
                return vv;
            }
            tmp.clear();
            tmp.push_back(1);
            tmp.push_back(1);
            vv.push_back(tmp);
            if (n==2){
                return vv;
            }
            for (int i=2;i<n;++i){
                //tmp.clear();
                //build tmp
                vector<int>& prevrow=vv[i-1];
                tmp=getNextRowInPascalTriangle(prevrow);
                vv.push_back(tmp);
            }
            return vv;
        }
    }

    bool test(){
        VVINT triangle;
        vector<int> tmp(1,2);
        triangle.push_back(tmp);
        tmp.clear();
        tmp.push_back(0);
        tmp.push_back(4);
        triangle.push_back(tmp);
        tmp.clear();
        tmp.push_back(6);
        tmp.push_back(5);
        tmp.push_back(0);
        triangle.push_back(tmp);
        tmp.clear();
        tmp.push_back(-1);
        tmp.push_back(1);
        tmp.push_back(4);
        tmp.push_back(3);
        triangle.push_back(tmp);

        int mi=minimumTotal(triangle);
        cout << mi << endl;

        VVINT tri=generatePT(5);
        return true;
    }

}




#endif