//Searching a 2D Sorted Matrix
#ifndef __ALGO_MATRIX__
#define __ALGO_MATRIX__

// 2013-08-25

namespace matrix{

    

    // search in a 2-D sorted array O(N)
    bool SearchIn2DMatrix(int m[][5], int target){
        int i=4;
        int j=0;
        while(i>=0 && j>=0 && i<5 && j<5){///
            if (target==m[i][j]){
                return true;
            }else if(target<m[i][j]){
                --i;
            }else{
                ++j;
            }
        }
        return false;
    }

    // advantage of vector<vector<int>> over array[][] is the former can have
    // different length of sub-vector
    void test(){
        int m[5][5]={
            {1,4,7,14,15},
            {2,5,8,12,19},
            {3,6,9,16,22},
            {10,13,14,17,24},
            {18,21,23,26,30},
        };

        bool b=SearchIn2DMatrix(m,16);
        cout << (b?"bingo!":"Not found!") << endl;
    }



}

#endif