#pragma once


namespace statistics{

    /// integer overflow?
    ///@todo overflow?
    long long factorial_array[1<<7]={1};//gobal initialized section
    long long factorial_dyn(int n){
        if (factorial_array[n]==0){
            long long r=1;
            int loopinvariant=1;
            while (loopinvariant<=n){
                r *= loopinvariant;
                factorial_array[loopinvariant++]=r;
            }
            return r;
        }else{
            return factorial_array[n];
        }
    }

    ///@brief factorial(n)/factorial(n-k)/factorial(k)
    long long choose(int n,int k){
#if 0
#if _DEBUG
        cout << factorial_dyn(n) << endl;
        cout << factorial_dyn(n-k) << endl;
#endif
        return factorial_dyn(n)/factorial_dyn(n-k)/factorial_dyn(k);
#else
        if (n==k){return 1;}
        int ma,mi;
        if (n-k > k){
            ma = n-k;
            mi = k;
        }else{
            ma = k;
            mi = n-k;
        }
        /// accumulation from 1 to mi times (accumulation from ma+1 to n)
        int r=1;
        for(int i=ma+1;i<=n;i++){// starting from 2 coz 1*x==x
            r*=i;
        }
        for (int i=2;i<=mi;i++){// starting from 2 coz 1*x==x
            r/=i; // ensure r is integer?
        }
        return r;
#endif
    }

    ///@param n - number of the persons
    ///@return the probability of "EXACTLY" 1 pair have the same DOB
    double birthdayparadox2(int n){
        if (n==1){
            return 0;
        }
        static const int ndayperyear=366;
        double r=choose(n,2)*1.0/ndayperyear;
        for (int i=1;i<=n-2;i++){
            r *= (ndayperyear-i)*1.0/ndayperyear;
        }
        return r;
    }

    ///@link http://en.wikipedia.org/wiki/Birthday_problem
    ///@brief at least 1 pair have the same DOB
    ///the flip side is all are different!!! so 1-P(all are different)
    double birthdayparadox(int n){
        if (n==1){
            return 0;
        }
        static const int ndayperyear=366;
        double r=1.0;
        for (int i=2;i<=n;i++){
            r = (ndayperyear-i+1)*r/ndayperyear;
        }
        return 1-r;
    }

    ///three color(r,y,b), draw three times with equal probability for the three colors
    ///given one of the three draws is red, what is the probability of exact one is yellow?
    ///original probability space: 3*3*3=27
    ///conditional probability space: [1- P(no red at all)]*27 = 19
    ///scenarios: 1y 2r; 1y 1r 1b

    bool test(){
        cout << choose(23,3) << endl;
        cout << choose(33,5) << endl;
        cout << choose(23,2) << endl;
        for(int i=1;i<60;i++){
            cout << i << "\t" << birthdayparadox(i) << endl;
        }
        cout << "------------------------" << endl;
        for(int i=1;i<60;i++){
            cout << i << "\t" << birthdayparadox2(i) << endl;
        }
        return true;
    }

}


