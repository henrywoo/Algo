#include "util.h"



vector<string> split(const string& str_,char delim){
    if (str_.empty()){
        return vector<string>();
    }
	string str(str_);
	vector<string> v;
	int pos=str.find(delim);
	if (pos==string::npos){
		v.push_back(str);
		return v;
	}
	while(pos!=string::npos){
		string str1=str.substr(0,pos);
		v.push_back(str1);
		str=str.substr(pos+1,str.size()-pos-1);
		pos=str.find(delim);
	}
	v.push_back(str);
	return v;
}

///@brief failure function of KMP
///@test "ababa" should return [0,0,1,2,3] the mymax length of the string \n
///which is both prefix and suffix of the string
///\todo
vector<int> failurefunc(const string& str){
	vector<int> v;
	return v;
}

///@brief implementation of KMP algo
///@return the pointer to the position of patterns inside original str
///@test
///(str="oaoaoq",pattern="oao") returns [str+0]\n
///(str="aboaobboaoq",pattern="oao") returns [str+2,str+7]
///\todo
vector<char*> findpattern(const string& str, const string& pattern){
	vector<char*> v;
	return v;

}

/// StringReplace("","xyz","abc")   return string("")
/// StringReplace("opq","","abc")   return string("opq")
/// StringReplace("opq","abc","")   return string("opq")
/// StringReplace("opq","o","")     return string("pq")
/// StringReplace("oapoaq","oa","abc") return string("abcpabcq")
/// StringReplace("oaoaoq","oao","abc") return string("abcaoq")
string StringReplace(const string& str, const string& pattern, const string& newstr){
	if (str.empty() || pattern.empty()){return str;}
	vector<char*> v=findpattern(str,pattern);
	if (v.empty()){
		return str;
	}else{
		int pn=v.size();//the number of patterns we found in str
		string s;
		int szdiff=newstr.size()-pattern.size();
		s.reserve(str.size()+pn*szdiff);
		for (vector<char*>::iterator i=v.begin();i!=v.end();++i){
			//TODO
		}
		return s;
	}
}

/// O(logN)
int power(unsigned int base, unsigned int exponent){
    int counter=0;
    if (exponent==0 || base==1){return 1;}
    if (base==0){return 0;}
    if (exponent==1){return base;}
    vector<int> p(exponent,0);
    //int *p=static_cast<int*>(malloc(exponent*sizeof(int)));// an array to store tmp result p[0]=1 p[2]=a**2
    //memset(p,0,exponent*sizeof(int));
    p[0]=1;
    p[1]=base;
    int e=2;
    int r=base;
    while (e<=exponent){
        r *= p[e>>1];// 0, 1, 2, 4
        counter++;
        if (e==exponent){
            cout << "number of multiplication:" << counter << endl;
            return r;
        }
        p[e] = r;
        e <<= 1;
    }
    e>>=1;
    int x=e;// for usage below

    int tmp=exponent-e; // e=8 tmp=7 e-tmp=1
    while (p[tmp]==0){
        if (e>tmp && p[e-tmp]!=0){
            r *= p[e] / p[e-tmp];
            counter++;
            cout << "number of multiplication:" << counter << endl;
            return r;
        }else if(e<tmp && p[tmp-e]!=0){
            r *= p[e] * p[tmp-e];
            counter++;
            cout << "number of multiplication:" << counter << endl;
            return r;
        }
        while (tmp-e<0){
            e>>=2;
        }//e=4,2
        r*=p[e];//* 4, * 2
        counter++;
        p[x+e]=r;
        tmp-=e;// self decrement
    }
    r*=p[tmp];//p[1]
    counter++;
    cout << "number of multiplication:" << counter << endl;
    //free(p);
    return r;
}

unsigned int Power(unsigned int n, unsigned int p){
    unsigned odd = 1; //oddk用来计算“剩下的”乘积
    int counter=0;
    while (p > 1){ // 一直计算到指数小于或等于1
        if (( p & 1 )!=0){ // 判断p是否奇数，偶数的最低位必为0
            odd *= n; // 若odd为奇数，则把“剩下的”乘起来
            counter++;
        }
        n *= n; // 主体乘方
        counter++;
        p >>= 1; // 指数除以2
        
    }
    counter++;
    cout << "number of multiplication:" << counter << endl;
    return n * odd; // 最后把主体和“剩下的”乘起来作为结果
}

bool testpower(){
    for(int i=0;i<50;i++){
        cout << "old" << Power(2,i) << endl;
        cout << "new" << power(2,i) << endl;
    }
    return true;
}


double getmean(double *p, size_t sz){
	double *end=p+sz;
	double *tmp=p;
	int i=1;
	double mean=*p;
	while (++p < end){
		mean = mean * i/(i+1) + *p/(i+1);
		i++;
	}
	return mean;
}

double maxsubarray_fixed(double *p, size_t sz, double*& end){
	int j=0;
	double maxsum=INT_MIN;
	double tmp=0;
	for (int i=0;i<sz;i++){
		tmp+=p[i];// p[i] == *(p+i)
		if (maxsum<tmp){
			maxsum=tmp;
			j=i;
		}
	}
	end=p+j;
	return tmp;
}

double maxsubarray_fixed_reverse(double *p, size_t sz, double*& end){
	if (sz==0)
	{
		return INT_MIN;
	}
	int j=0;
	double maxsum=INT_MIN;
	double tmp=0;
	for (int i=0;i<=sz;i++){
		tmp+=*(p-i);// *(p+i) and p[i] difference - i can be negative in case of p+i
		if (maxsum<tmp)
		{
			maxsum=tmp;
			j=i;
		}
	}
	end=p-j;
	return maxsum;
}


///@brief Largest Sum Contiguous Subarray
///@param
/// start - pointer to start of mymax subarray\n
/// end   - pointer to end of mymax subarray\n
///@remarks bottom up dynamic programming
double maxsubarray(double *p, size_t sz, double*& start, double*& end)
{
	if (sz==0){return INT_MIN;}
	double totalsum=INT_MIN;
	start=end=p;
	for (int i=0;i<sz;i++)
	{
		if (end+1!=p+i){// not adjacent
			if (totalsum>p[i])
			{
				//pass
			}
			double* tmp1=NULL;
			double tmp2=maxsubarray_fixed_reverse(p+i,(p+i-start),tmp1);
			if (totalsum>tmp2)
			{
				//pass
			}
			if (totalsum<tmp2)
			{
				totalsum=tmp2;
				start=tmp1;
				end=p+i;
			}
			if (totalsum<p[i])
			{
				totalsum=p[i];
				start=end=p+i;
			}
		}else{//adjacent
			if (totalsum>p[i])
			{
				//pass
			}
			if (totalsum<p[i])
			{
				totalsum=p[i];
				start=end=p+i;
			}
			if (totalsum<totalsum+p[i])//p[i]>0
			{
				totalsum=totalsum+p[i];
				end=end+1;
			}
		}
	}
	return totalsum;
}

namespace util{
	void test(){
#if 0
		char pp[3]="1a";
		char c=*pp+1;
		cout << c << endl;
		double test[8]={-0.2,-0.3,0.4,-.1,-.2,.1,.8,-.3};
		double test2[]={-0.2};
		double *p=NULL;
		double mf=maxsubarray_fixed(test,sizeof(test)/sizeof(double),p);
		cout<< mf << endl;
		copy(test,p+1,ostream_iterator<double>(cout," "));
#endif
		double test[8]={-0.2,-0.3,0.4,-.1,-.2,.1,.5,-.3};
		double * head, * tail;
		double totalsum=maxsubarray(test,sizeof(test)/sizeof(double),head,tail);
		copy(head,tail+1,ostream_iterator<double>(cout," "));

	}

	void swap(int& a,int& b){
		if (a!=b){
			a=a^b;
			b=a^b;
			a=b^a;
		}
	}
}