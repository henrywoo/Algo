#ifndef __ALGO_BIGINT__
#define __ALGO_BIGINT__

#include <time.h>
#include <math.h>

typedef unsigned long long ULL;

namespace bigint{

    ///big integer multiplication
    ///naive method
    vector<unsigned char> BigIntMultiply1(vector<unsigned char> lhs,
        vector<unsigned char> rhs)
    {
        int sz1=lhs.size();
        int sz2=rhs.size();
        vector<unsigned char> result(sz1+sz2,0);// sz1+sz2-1 copies of 0

        reverse(lhs.begin(),lhs.end());// reverse the bigint
        reverse(rhs.begin(),rhs.end());

        // 345(lhs) * 7(rhs) = 2415 ==> 35 28 21 => 5 => 5 31 21=> 5 1 24 => 5 1 4 2
        for (int i=0;i<sz1;++i){
            for (int j=0;j<sz2;++j){
                result[i+j] += lhs[i] * rhs[j];///
            }
        }
        int carry=0;
        for (int j=0;j<sz1+sz2;++j){///
            carry = result[j] / 10;
            result[j] = result[j] % 10;
            if(j+1<sz1+sz2){
                result[j+1] += carry;///carry to higher digit
            }
        }
        if (result.back()==0){
            result.pop_back();
        }
        std::reverse(result.begin(),result.end());
        return result;
    }

    ///big integer multiplication - just return LSB k digits
    ///naive method
    vector<unsigned char> BigIntMultiply2(vector<unsigned char> lhs,
        vector<unsigned char> rhs, int k)
    {
        int sz1=lhs.size();
        int sz2=rhs.size();

        if (k>sz1+sz2){///
            k=sz1+sz2;
        }
        vector<unsigned char> result(k,0);// sz1+sz2-1 copies of 0

        reverse(lhs.begin(),lhs.end());// reverse the bigint
        reverse(rhs.begin(),rhs.end());

        // 345(lhs) * 7(rhs) = 2415 ==> 35 28 21 => 5 => 5 31 21=> 5 1 24 => 5 1 4 2
        for (int i=0;i<sz1;++i){
            for (int j=0;j<sz2;++j){
                if (i+j<k){
                    result[i+j] += lhs[i] * rhs[j];///
                }
                
            }
        }
        int carry=0;
        for (int j=0;j<k;++j){///
            carry = result[j] / 10;
            result[j] = result[j] % 10;
            if(j+1<k){
                result[j+1] += carry;///carry to higher digit
            }
        }

        if (result.back()==0){
            result.pop_back();
        }

        std::reverse(result.begin(),result.end());
        return result;
    }

    //写一个程序,找出 5^1234566789893943的从底位开始的1000位数字
    //ULONG_MAX;INT_MAX;

    vector<unsigned char> GetLSBofPower(vector<unsigned char>& base,
        ULL exponent, int k)
    {
        assert(exponent==1234566789893943);
        // divide and conquer bottom up
        if (exponent==0){
            return vector<unsigned char>(1,1);
        }else if(exponent==1){
            return base;
        }else{
            vector<unsigned char> result=vector<unsigned char>(1,1);
            while(exponent){
                if (exponent&1){
                    result=BigIntMultiply2(result,base,k);
                }
                base=BigIntMultiply2(base,base,k);
                copy(base.begin(),base.end(),ostream_iterator<unsigned int>(cout,""));cout<<endl;
                exponent>>=1;
            }
            return result;
        }
    }


    void test(){
        //small case
        {
            unsigned char a[]={3,4,5};
            unsigned char b[]={9,7};
            vector<unsigned char> v1(a,a+_countof(a));
            vector<unsigned char> v2(b,b+_countof(b));
            vector<unsigned char> r=BigIntMultiply2(v1,v2,3);
            copy(r.begin(),r.end(),ostream_iterator<int>(cout," "));cout << endl;
        }
        //big case
        // verification: https://defuse.ca/big-number-calculator.htm
        {
            vector<unsigned char> v1(200,0);
            srand(time(NULL));
            for (int i=0;i<v1.size();++i){
                v1[i]=rand()%10;
            }
            vector<unsigned char> v2=v1;
            random_shuffle(v2.begin(),v2.end());
            v2.resize(100);
            copy(v1.begin(),v1.end(),ostream_iterator<int>(cout,""));
            cout << endl;
            copy(v2.begin(),v2.end(),ostream_iterator<int>(cout,""));
            cout << endl;

            vector<unsigned char> r=BigIntMultiply1(v1,v2);
            copy(r.begin(),r.end(),ostream_iterator<int>(cout,""));
            cout << endl;
        }

        {
            vector<unsigned char> vi(1,5);
            unsigned long long ull=ULLONG_MAX;
            GetLSBofPower(vi, 1234566789893943,1000);
        }
        
    }

}


#endif