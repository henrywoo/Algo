#ifndef __ALGO_BOGGLE2__
#define __ALGO_BOGGLE2__

namespace MS{
    /*
    1. 给一个array，size n, 里面每个数字的range 是 1～n, 输出 duplicates 以及 每
    个duplicate出现的次数  要 O(1) 空间
    2. rotate a image
    3. 一个城市有1百万人，里面有100个坏人，有一个摄像头，能够看出一个人是不是坏
    人，但是它有1%的可能性说错，既把好人说成坏人，或者把坏人说成好人，问当一个摄
    像头报警的时候，那个人有多少可能是个坏人
    4.有一个色子，6个面，4个面是绿色，2个是红色，现在丢1000次，给三个pattern （1
    ） grrgrrrg, (2) rrgrrrg  (3) grrgggr, 给这三个pattern出现的概率 排序
    5. 给一棵树，非binary， 输出一个节点的 sibling
    6. 给一个array 都是 positive， 给一个 sum， 输出所有的 subarray 加和是 sum
    7. 给2个 array A， B， 写一个function 输出 在A，不在B的
    8. 解释 hash table 是怎么实现的， 解释外部排序
    */

    //1. every number is in the range of [1~n] is a very important hint! 
    //swap the array[n-1] with tmp repeatedly

    // 6.
    
    /// continuous sub-array
    vector<vector<int>> GetSubArray(int *a, int sz, int sum){
        vector<vector<int>> vvi;
        for (int i=0;i<sz;++i){
            if (a[i]>sum){
                continue;
            }
            int tmp=0;
            for (int j=i;j<sz;++j){
                tmp += a[j];
                if (tmp==sum){
                    vector<int> vi(a+i,a+j+1);
                    vvi.push_back(vi);
                    break;
                }
            }
        }
        return vvi;
    }

    /// non-continuous sub-array backtrack method
    vector<vector<int>> GetSubArray2(int *a, int sz, int sum){
        vector<vector<int>> vvi;
        vector<int> iVec;
        for (int i=0;i<sz || !iVec.empty();){
            //op1
            //if (a[i]>sum){continue;}
            while(i < sz){
                if(sum >= a[i]){
                    iVec.push_back(i);
                    sum -= a[i];
                }
                i += 1;
            }
            if(0 == sum){
                vvi.push_back(iVec);
            }
            i = iVec.back();
            iVec.pop_back();

            sum += a[i++];
        }
        return vvi;
    }

    void finddup(int a[],int sz){
        for (int i=0;i<sz;++i){
            cout << a[i] << endl;
        }
    }

    bool test1(){
        int a[]={1,1,1,9,9,9,8,8,8,2,2,2};
        finddup(a,_countof(a));
        int b[]={1,2,1,3,5,11,10,8,6,2,21,5};
        vector<vector<int>> vvi=GetSubArray2(b,_countof(b),29);
        cout << vvi.size() << endl;
        for (int i=0;i<vvi.size();++i){
            for (int j=0;j<vvi[i].size();++j){
                cout << b[vvi[i][j]] << " "; 
            }
            cout << endl;
        }
        return true;
    }

    namespace boggle2{

    void test(){
        char c[]="ASSDS";
        char cc[][6]={
            "ASSDS",
            "SBEYB",
            "GFOIF",
            "HUUKA",
            "SYEMD"
        };

    }

}
}


#endif