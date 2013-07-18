#include "util.h"



vector<string> split(const string& str_,char delim){
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
		if (maxsum<tmp)
		{
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
/// start - pointer to start of max subarray
/// end   - pointer to end of max subarray
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
		a=a^b;
		b=a^b;
		a=b^a;
	}
}