#ifndef __ALGO_BITOPS__
#define __ALGO_BITOPS__

#include "util.h"

#define MOD(x,y) (x%y)

namespace bitops{

    /*
    ~0 == -1
    ~（~0 << n） // 实现最低n位为1，其余位为0的位串信息
    (x >> (1+p-n)) & ~(~0 << n) // 截取变量x自p位开始的右边n位的信息
    new |= ((old >> row) & 1) << (15 – k) // 截取old变量第row位，并将该位信息装配到变量new的第15-k位
    s &= ~(1 << j) // 将变量s的第j位置成0，其余位不变
    for(j = 0; ((1 << j) & s) == 0; j++) ; // 设s不等于全0，代码寻找最右边为1的位的序号j
    */
    /// and &; or |; xor ^;
    unsigned int ReversePositiveIntBit(unsigned int posi);

    int ReverseIntBit(int i){
        if (i==0){
            return 0;
        }else if(i<0){
            return -1*ReversePositiveIntBit(-i);
        }else{
            return ReversePositiveIntBit(i);
        }
    }

    unsigned int ReversePositiveIntBit(unsigned int posi){
        int INTBITNUM=sizeof(posi)*8 - 1; // minus the first sign bit
        int r=0;
        for (int i=1; i<=INTBITNUM; i++){// i - the ith bit
            int tmp=posi >> (i-1);// i-1 is the bit number we need to shift
            if (tmp & 1){ // if the 1st bit is 1, then the 31st bit is 1
                r |= (1 << (INTBITNUM - i));// wrong: r |= (1 << (INTBITNUM + 1 - i));
            }
        }
        return r;
    }

    ///Note: O(K) - K is the number of Bit 1 in i
    int BitCount(int i){
        int count=0;
        while(i){//i!=0
            i &= i-1;// the purpose is to remove the last Bit 1 in i
            count++;
        }
        return count;
    }

    void printBit(int i, bool fixedlength=true){
        if (fixedlength){
            printf(i<0?"1":"0");
            for (int count=0;count<31;count++){
                int j = i & 1;
                printf("%d",j);
                i >>= 1;
            }
            printf("\n");
        }else{
            while(i!=0){
                int j = i & 1;
                printf("%d",j);
                i >>= 1;
            }
            printf("\n");
        }
        
    }

    ///@param i- input number; n- rotate number of bits
    ///@brief rotate(00010b)
    int rotate(int i, int n){
        assert(i>0);
        int x=i >> n;
        i <<= (sizeof(int)>n)?(sizeof(int)-n):0;
        return x|i;
    }


    void test(){
        int i=INT_MAX;
        printBit(i);
        printBit(i<<1);
        printBit(-1);
        printBit(-1>>1);
        cout << (i<<1) << endl;
        cout << (i*2) << endl;
        cout << (1>>1) << endl;
        cout << (-1>>1) << endl;


        int r=ReverseIntBit(127);
        cout << r << endl;
        printBit(r);
        r=ReverseIntBit(r);
        cout << r << endl;
        printBit(r);

        vector<bool> vb(100,false);//100 copies of false
        reverse(vb.begin(),vb.end());
        vb.flip();
        copy(vb.begin(),vb.end(),ostream_iterator<bool>(cout,","));
    }

}



#endif