/// namespace of dynamic programming
#include <string>
#include <deque>
#include <iostream>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <hash_set>
#include "heap.h"
#include "util.h"

namespace Augmentation{

    namespace kthmax{
        ///@todo
        ///1. kth mymax in a given unsorted array
        ///2. kth mymax in two given unsorted arrays
        ///3. kth mymax in x given unsorted/sorted arrays
        ///http://www.mitbbs.com/article_t/JobHunting/32494041.html
    }

#if 0 // TODO - difficult
	class SlidingWindowWithMax{
	private://implementation
		deque<int> d;
		const int capacity;
		void push_back_safe(int i){
			if (d.size()>=capacity){
				d.pop_front();
			}
			d.push_back(i);
		}

	public://interface
		SlidingWindowWithMax(int cap):capacity(cap){}
		bool push_back(int newnode){
			if (d.empty()){
				push_back_safe(newnode);
			}else{
				if (newnode > d.back()){
					pop_back();
				}
				push_back_safe(newnode);
			}
			return true;
		}

		void pop_back(){d.pop_back();}
		void pop_front(){d.pop_front();}
		int front(){return d.front();}
		int size(){return d.size();}
	};
#endif

	///http://codercareer.blogspot.sg/2012/02/no-33-maximums-in-sliding-windows.html
	template<class T>
	T GetMaxOfSet(const set<T>& se){
        //set<T>::iterator i=se.end();
        //--i;
		return *(--se.end());// refer to code of back() for vector
	}


	///@brief Maximums in Sliding Windows
	///Time Complexity -> All cases: O(N*logM)
	///[N-size of the big array; M-size of the window]
	template<class T>
	vector<T> MaxInSlidingWindows(T* head, T* tail, int window_sz){
		vector<T> v;
		set<T> se; // should be priority_queue, but unfortunately STL's PQ doesn't provide remove
		
		// initial setup - O(M)
		for (int i=0;i<=window_sz-1;i++){
			se.insert(head[i]);
		}
		v.push_back(GetMaxOfSet(se));

		// (N-M)*2*logM
		T* tmppush=head+window_sz;// the item after the sz'th item
		T* tmpdele=head;
		while (tmppush<=tail){
			se.insert(*tmppush); //O(logN)
			se.erase(*tmpdele);  //O(logN)
			v.push_back(GetMaxOfSet(se));
			
			++tmppush;
			++tmpdele;
		}

		return v;
	}

    /// function has no template specialization or partial specialization
    vector<int> MaxInSlidingWindows(int* head, int* tail, int window_sz)
    {
        vector<int> v;
        heaping::heap maxh;
        // initial setup - O(M)
        for (int i=0;i<=window_sz-1;i++){
            maxh.insert(head[i]);
        }
        v.push_back(*maxh.head());
        // (N-M)*2*logM
        int* tmppush=head+window_sz;// the item after the sz'th item
        int* tmpdele=head;
        while (tmppush<=tail){
            maxh.insert(*tmppush); //O(logN)
            maxh.erase(*tmpdele);  //O(logN)
            v.push_back(*maxh.head());
            ++tmppush;
            ++tmpdele;
        }
        return v;
    }

	vector<int> maxInWindows(const vector<int>& numbers, int windowSize){
		vector<int> maxInSlidingWindows;
		if(numbers.size() >= windowSize && windowSize > 1)
		{
			deque<int> indices;
			//preprocessing
			for(int i = 0; i < windowSize; ++i)
			{
				while(!indices.empty() && numbers[i] >= numbers[indices.back()])
					indices.pop_back();

				indices.push_back(i);
			}

			for(int i = windowSize; i < numbers.size(); ++i){// O(N)
				maxInSlidingWindows.push_back(numbers[indices.front()]);

				while(!indices.empty() && numbers[i] >= numbers[indices.back()]) //O(m-1)
					indices.pop_back(); // O(1)

				if(!indices.empty() && indices.front() <= i - windowSize)
					indices.pop_front(); // O(1)

				indices.push_back(i); // O(1)
			}
			maxInSlidingWindows.push_back(numbers[indices.front()]);
		}

		return maxInSlidingWindows;
	}

	void maxSlidingWindow(int A[], int n, int w, int B[]) {
		deque<int> Q;
		for (int i = 0; i < w; i++) {//O(w)
			while (!Q.empty() && A[i] >= A[Q.back()])
				Q.pop_back();
			Q.push_back(i);
		}
		for (int i = w; i < n; i++) {//O(n)
			B[i-w] = A[Q.front()];
			while (!Q.empty() && A[i] >= A[Q.back()])//O(w)
				Q.pop_back();
			while (!Q.empty() && Q.front() <= i-w)//O(n-w)
				Q.pop_front();
			Q.push_back(i);
		}
		B[n-w] = A[Q.front()];
	}

	


	bool test(){
#if 0
		int c[]={9,6,5,8,2};
		SlidingWindowWithMax swm(2);
		int* tmp=c;
		while (tmp!= c+ARRAYSIZE(c,int)-1){
			swm.push_back(*tmp);
			cout << swm.front() << endl;
			tmp++;
		}
#endif

		int a[]={2, 3, 4, 2, 6, 2, 5, 1};
		vector<int> v=MaxInSlidingWindows<int>(a,a+_countof(a)-1,3);
        //4 4 6 6 6 5
		copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));cout << endl;

		int b[]={9,7,8,6,5,4,3,2,1};
		vector<int> ip(b,b+_countof(b)-1);
		v=maxInWindows(ip, 1);//requires size of SD>1
		copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));cout << endl;
		return true;
	}
	
}

namespace addTwoArbitraryNumbers_{
	/*You are given two linked lists representing two non-negative numbers. 
	///The digits are stored in reverse order and each of their nodes contain 
	///a single digit. Add the two numbers and return it as a linked list.
	Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
	Output: 7 -> 0 -> 8*/
	struct ListNode {//single linked list
		int val;
		ListNode *next;
		ListNode(int x) : val(x), next(NULL) {}
	};

	int AddTwoDigits(int x,int y,bool& carrybit/*in&out*/){
		int r=x+y+(carrybit?1:0);
		carrybit=false;
		if (r>=10){//overflow handling
			carrybit=true;// cannot be greater than 1?
			r-=10;
		}
		return r;
	}

	
	ListNode *addTwoArbitraryNumbers(ListNode *l1, ListNode *l2){ // O(n)
		if (NULL==l1 || NULL==l2){
			return NULL;
		}
		ListNode *head1=l1;
		ListNode *head2=l2;
		int index1(0),index2(0);
		bool carrybit=false;
		/// loop until end of l1 - size(l1)> == < size(l2) ? 
		ListNode* lastnode=head1;// in case of 5+5 or 91+9 or 9+91
		do{// use do-while when looping slist??!!
			int& v1=head1->val;
			if(head2){
				v1=AddTwoDigits(v1,head2->val,carrybit);
				head2=head2->next;
				index2++;
				if (index2>index1){
					index2--;// force index1==index2
				}
			}
			//sizel1 > sizel2
			if (index1>index2){
				if (carrybit){
					v1=AddTwoDigits(v1,0,carrybit);
				}else{// all the work has been done here!
					break;// we may return l1 here actaually
				}
			}

			lastnode=head1;
			head1=head1->next;
			index1++;
		}while(head1);
		// size l2 > size l1
		if (head2){
			lastnode->next= head2;
			do{
				//overflow handling
				head2->val = AddTwoDigits(head2->val,0,carrybit);
				lastnode=head2;
				head2=head2->next;
			}while(head2);
		}
		if (carrybit){
			ListNode* tmp=new ListNode(1);
			lastnode->next=tmp;
		}
		return l1;
	}

	bool test(){
		ListNode n1(0);
		
		ListNode n3(7);ListNode n2(3);n3.next=&n2;
		//addTwoArbitraryNumbers(NULL, NULL);
		addTwoArbitraryNumbers(&n1, &n3);
		//NULL+NULL
		//0+NULL
		//NULL+0
		//9+1
		//99+1
		return true;
	}
}

namespace integerrelated{

#if 0
	int power(int x,int y){
		int r=1;
		while (y>0){
			r*=x;y--;
		}
		return r;
	}
	
	int getlastNdigit(int x,int n){
		if (n==1){
			return x%10;
		}
		int tmp=power(10,n);
		if (x<tmp/10){
			return -1;
		}
		return x-x/tmp*tmp;
	}
#endif

	int reverse(int x) {// 10 -> 1
		int sign=1;
		if(x<0){x*=-1;sign=-1;}
		if(x<10)return x*sign;
		int r=0;//result
#if 1
		// x%10; x/10; x/10<10 // 12
		do{
			r = r*10 + x%10; // 2->21
			x = x/10;//1->0
		} while (x);
#else
		stack<int> stk;
		int n=1;
		while(true){
			int tmp=getlastNdigit(x,n); // x%10 - last one; 
			if (tmp<0){
				break;
			}
			tmp=tmp/power(10,n-1);
			stk.push(tmp);
			n++;
		}
		r=0;int n=0;
		while (!stk.empty()){
			r += stk.top()*power(10,n);
			n++;
			stk.pop();
		}
#endif

		r=sign*r;
		return r;
	}

#define CHAR2INT(c) (c-'0')

	int myatoi(const char* cc){
		if(cc==NULL) return 0;
		if(*cc=='\0') return 0;
		int sign=1;
		bool signedalready=false;
		stack<int> stk;
		for (int i = 0; i <= strlen(cc)-1; i++){
			char c=cc[i];
			// TODO: add sign handling block here
			if(c=='+'){
				if(CHAR2INT(cc[i+1])>=10 || CHAR2INT(cc[i+1])<0){break;}
				if(signedalready) return 0;
				signedalready=true;
				continue;
			}else if(c=='-'){
				if(CHAR2INT(cc[i+1])>=10 || CHAR2INT(cc[i+1])<0){break;}
				if(signedalready) return 0;
				sign=-1;signedalready=true;
				continue;
			}else if(c==' '){
				if(stk.empty())
					continue;
				else{break;}
			}else{
				int digit=CHAR2INT(c);
				if (digit>=10 || digit<0){
					break;
				}
				stk.push(digit);
			}
		}
		long long result=0,n=0;
		while(!stk.empty()){
			int digit=stk.top();stk.pop();
			result+=pow(10,n)*digit;
			n++;
		}
		result*=sign;
		if(result>INT_MAX){return INT_MAX;}
		if(result<INT_MIN){return INT_MIN;}
		return result;
	}

    bool isPalindromeInt(int x, int &y) {
        if (x < 0) return false;
        if (x == 0) return true;
        if (isPalindromeInt(x/10, y) && (x%10 == y%10)) {
            y /= 10;
            return true;
        } else {
            return false;
        }
    }
    bool isPalindromeInt(int x) {
        return isPalindromeInt(x, x);
    }

	void test(){
        isPalindromeInt(3123);
		int tmp=12309002;
		cout << tmp << endl;
		tmp=reverse(tmp);
		cout << tmp << endl;
		tmp=10000;
		cout << tmp << endl;
		tmp=reverse(tmp);
		cout << tmp << endl;
		//cout << myatoi("2147483648") << endl;
	}


}

namespace stringrelated{

	/// O(N*M) [N- size of vector; M- size of longest prefix]
	string longestCommonPrefix(vector<string> &strs) {
		if(strs.empty()) return string();
		if(strs.size()==1) return strs[0];

		//at least two strings inside
		string r;
		int sz=strs.size();
		int j=1;//size of the string
		while(true){
			char c=strs[0][j-1];
			bool allthesame=false;
			for(int i=1;i<=sz-1;++i){
				if(strs[i].size()<j){goto longestCommonPrefix_DONE;}//jump out of multiple loops with goto
				if(c!=strs[i][j-1]){
					goto longestCommonPrefix_DONE;
				}
			}
			r.push_back(c);
			j++;
		}
longestCommonPrefix_DONE:
		return r;
	}


	/// worst O(N^2*M) [N - the size of string; M- the size of longest common substr]
	///http://www.geeksforgeeks.org/longest-common-substring/
	string longestCommonSubstr(const string &s1, const string &s2){
		string r;
		int sz1=s1.size();
		int sz2=s2.size();
		for (int i=0;i<=sz1-1;i++){//i - index for s1; O(n^2)
			int tmp1size=sz1-1-i+1;
			if (tmp1size < r.size()){//optimization
				break;
			}
			string tmp1=s1.substr(i,tmp1size);
			int j=0;//j-index for s2
			while(j<=sz2-1){///O(n*m)
				int tmp2size=sz2-1-i+1;
				if (tmp2size < r.size()){//optimization
					break;
				}
				string tmp2=s2.substr(j,tmp2size);
				vector<string> v;
				v.push_back(tmp1);
				v.push_back(tmp2);
				string tmpr=longestCommonPrefix(v);//O(m)
				if (tmpr.size()>r.size()){
					r=tmpr;
				}
				j++;
			}
		}
		return r;
	}

	string longestCommonSubstr(vector<string> &strs) {
		string r;

		return r;
	}

	/// Dynamic Programming
	string longestCommonSubseq(const char* head1,
		const char* tail1,
		const char* head2,
		const char* tail2)
	{
		static int count=0;
		count++;
		cout<< count <<" ";
		copy(head1,tail1+1,ostream_iterator<char>(cout,""));cout<<" ";
		copy(head2,tail2+1,ostream_iterator<char>(cout,""));cout<< endl;
		int sz1=tail1-head1+1;
		int sz2=tail2-head2+1;
		if (sz1==0||sz2==0)
		{return string();}
		if (*tail1==*tail2){
			string s=longestCommonSubseq(head1,tail1-1,head2,tail2-1);
			s.push_back(*tail1);
			return s;
		}else{
			string s1=longestCommonSubseq(head1,tail1-1,head2,tail2);
			if (s1.size()>=sz1 || s1.size()>=sz2-1){/// improvement
				return s1;
			}
			string s2=longestCommonSubseq(head1,tail1,head2,tail2-1);
			if (s1.size()>=s2.size()){
				return s1;
			}else{
				return s2;
			}
		}
	}

#if 0
    #define pointer2int int*
    #define pSubSeq list<vector<int*>>

	///@brief Longest monotonically increasing subsequence
	pSubSeq LMIS(int* head, int* tail){
		if (head==tail || head==NULL || tail==NULL){
			pSubSeq tmp;
			return tmp;
		}else if(1){
			int* tmptail=tail;
			pSubSeq p=LMIS(head,tmptail-1);

			for (pSubSeq::iterator i=p.begin();i!=p.end();i++){

			}

			if (*tail>*p.second){
				p.second=tail;
			}else{
				//TODO
			}
			return p;
		}
	}
#endif

    ///@todo
    bool IsScramble(const string& a,const string& b){
        return true;
    }

    ///@todo


	void test(){
		//int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
		//pSubSeq p=LMIS(a,a+ARRAYSIZE(a,int)-1);
		//string s3("ACCGGTCGAGTGCGCGGAAGCCGGCCGAA");
		//string s4("GTCGTTCGGAATGCCGTTGCTCTGTAAA");
		string s3("ACCGGTCGAGT");
		string s4("GTCGTTC");
		/*
		1519 ACCGGTCG GTCG
		1520 ACCGGTC GTC
		1521 ACCGGT GT
		1522 ACCGG G
		1523 ACCG
		*/
		string r2=longestCommonSubseq(&s3.at(0),&s3.back(),&s4.at(0),&s4.back());

		string s1("ABABC"),s2("BABCA");
		string r=longestCommonSubstr(s1, s2);
		cout<<r.c_str()<< endl;
	}
	
}


namespace DP{

	using namespace std;

	namespace Palindrome{
	/// substring -consecutive; subsequence - not necessarily consecutive

    ///@ O(n)
    bool ispalindromesubstr(const string& s){
            if(s.empty()){
                //return false;
                return true;///@note be careful of the boundary condition
            }else{
                if(s.size()==1){
                    return true;
                }else{
                    if(*s.begin()==s.back()){
                        return ispalindromesubstr(s.substr(1,s.size()-2));
                    }else{
                        return false;
                    }
                }
            }
    }

    bool ispalinstr(const string& s){
        if (s.empty() || s.size()==1){
            return true;
        }
        const char* ph=&*s.begin();
        const char* pt=&s.back();
        while(ph<pt){
            if(*ph++!=*pt--) return false;
        }
        return true;
    }


#define PSS pair<string,string>
    
    /// O(n)
    PSS Longest_Palindrome_Substring_fromindex0(const string& s){
        if (s.empty()){
            return PSS("","");
        }else{
            if (s.size()==1){
                return PSS(s,"");
            }else if(s.size()==2){
                if (s[0]==s[1]){
                    return PSS(s,"");
                }else{
                    return PSS(s.substr(0,1),s.substr(1,1));
                }
            }else{
                int pos=s.rfind(s[0]);
                if (pos==0){//string::npos is -1
                    return PSS(s.substr(0,1),s.substr(1,s.size()-1));
                }else{
                    ///@example aba, pos=2, substr(1,1)
                    if (ispalinstr(s.substr(1,pos-1-1+1))){
                        return PSS(s.substr(0,pos+1),s.substr(pos+1,s.size()-1-(pos+1)+1));
                    }else{
                        PSS pr=Longest_Palindrome_Substring_fromindex0(s.substr(0,pos-1));
                        return PSS(pr.first,
                            s.substr(pr.first.size(),s.size()-1-pr.first.size()+1));
                    }
                }
            }
        }
    }

    PSS Longest_Palindrome_Substring_fromend(const string& s){
        if (s.empty()){
            return PSS("","");
        }else{
            if (s.size()==1){
                return PSS("",s);
            }else if(s.size()==2){
                if (s[0]==s[1]){
                    return PSS("",s);
                }else{
                    return PSS(s.substr(0,1),s.substr(1,1));
                }
            }else{
                int pos=s.find(s.back());
                if (pos==s.size()-1){//string::npos is -1
                    return PSS(s.substr(0,s.size()-1),s.substr(s.size()-1,1));
                }else{
                    ///@example aba, pos=2, substr(1,1)
                    //if (ispalindromesubstr(s.substr(pos+1,s.size()-1-(pos+1)))){
                    if (ispalinstr(s.substr(pos+1,s.size()-1-(pos+1)))){
                        return PSS(s.substr(0,pos),s.substr(pos,s.size()-pos));
                    }else{
                        PSS pr=Longest_Palindrome_Substring_fromend(s.substr(pos+1,s.size()-pos-1));
                        return PSS(s.substr(0,pos+1+pr.first.size()),
                            pr.second);
                    }
                }
            }
        }
    }

    unordered_map<string,unsigned int> str2mc;
    ///@brief complexity??
    int minCut(const string& s){
        if (s.empty()){
            return 0;
        }else if(s.size()==1){
            return 0;//1;
        }else{
            int r=0x7fffffff;
            if(str2mc.find(s)==str2mc.end()){
                int sz=s.size();
                /// O(n*x)
                for(int i=0;i<=sz-1;i++){//i - index
                    string s1=s.substr(0,i);
                    string s2=s.substr(i,sz-i);
                    int extra=0;
                    if (!s1.empty()&&!s2.empty()){
                        extra=1;
                    }
                    PSS pr=Longest_Palindrome_Substring_fromindex0(s2);//O(n)
                    int incr=1;
                    if (pr.first.empty()||pr.second.empty()){
                        incr=0;
                    }
                    int tmp=minCut(s1)+incr+extra+minCut(pr.second);//????
                    if (r>tmp){
                        r=tmp;
                        if (tmp==0){
                            str2mc[s]=tmp;
                            return 0;
                        }
                    }
                }
            }else{
                return str2mc[s];
            }
            str2mc[s]=r;
            return r;
        }
    }

    ///????
    int minCut2(string s){
        int len = s.size();
        int *dp = new int[len];//pointer to array
        bool **p = new bool*[len];//2-D array to store boolean
        for(int i=0; i<len; i++)
            p[i] = new bool[len];//initialize 2-D array

        for(int i=0; i<len; i++) dp[i]=i;

        for(int i=0; i<len; i++){
            for(int j=0; j<len; j++){
                p[i][j] = (i == j);// assign true to the diagonal elements
            }
        }
        for(int i=1; i<len; i++){
            for(int j=0; j<=i; j++){
                if(s[i] == s[j] && (i-j<2 || p[j+1][i-1])){
                    p[j][i] = true;
                    if(j == 0){//??
                        dp[i] = 0;
                        break;
                    } else {
                        printf("%d = mymin(%d,%d+1) i=%d j=%d\n",dp[i],dp[i],dp[j-1],i,j);
                        dp[i] = mymin(dp[i],dp[j-1]+1);
                    }
                }
            }
        }
#ifdef _DEBUG
        for (int i=0;i<=len-1;i++){
            for (int j=0;j<=len-1;j++){
                printf("%d ",p[i][j]);
            }
            printf("\n");
        }
#endif
        return dp[len-1];
    }


    ///@brief flawless
    int minCut3(const string& s){
        if (s.empty()){
            return 0;
        }else if(s.size()==1){
            return 0;//1;
        }else{// at least s has two chars inside
            if(str2mc.find(s)==str2mc.end()){
                PSS pr=Longest_Palindrome_Substring_fromend(s);//first could be empty, second will be at least one char
                assert(pr.second.size()>=1);
                //when first is empty, return 0 immediately
                if(pr.first.empty()){
                    str2mc[s]=0;
                    return 0;
                }
                if(pr.second.size()==1){
                    int tmp=minCut3(s.substr(0,s.size()-1))+1;
                    str2mc[s]=tmp;
                    return tmp;
                }else{
                    int tmp=minCut3(pr.first);
                    //cout<< pr.first.c_str() << endl;
                    //cout << minCut3(pr.first) << endl;
                    if (tmp==0){//optimization
                        str2mc[s]=1;
                        return 1;
                    }
                    /*tmp=mymin(tmp+1,// because pr.second is a palin now
                        minCut3(s.substr(0,s.size()-1))+1);*/
                    tmp=tmp+1;
                    int sz=s.size();
                    for (int i=1;i<=sz-2;i++){// must not start from 0, or the recursive will be endless
                        int x=minCut3(s.substr(0,i));
                        if (x>=tmp){
                            break;
                        }
                        int y=minCut3(s.substr(i,sz-i));
                        if (x+y+1<tmp){
                            tmp=x+y+1;
                        }
                    }
                    str2mc[s]=tmp;
                    return tmp;
                }
            }else{
                return str2mc[s];
            }
        }
    }

	///@
	bool IsContainChar(const char* p,char const *q, const char c){
		while (p<=q){
			if (c==*p){
				return true;
			}
			p++;
		}
		return false;
	}


	bool HasPalindromeSubSequence(const string& str){
		return true;
	}


	///@brief "carac" is a subsequence of "character"
	bool IsSubSequence(const string& str,const string& subseq){
		return true;
	}


	/************************************************************************
	longest(i,j)= j-i if j-i<=1,
	if x[i]==x[j]
	  = 2+longest(i+1,j-1)
	else
	  = mymax(longest(i+1,j),longest(i,j-1))
	************************************************************************/
	///@brief top down method
	int Longest_Palindrome_SubSequence(const char* x,const char*y){
		static map<string,int> m;
		int r=0;
		if (y-x<=1){
			r=1;
		}else{
			string tmp(x,y+1);
			if (m.find(tmp)!=m.end()){
				return m[tmp];
			}
			if(*x==*y){
				r=2+Longest_Palindrome_SubSequence(x+1,y-1);
			}else{
				r=mymax(Longest_Palindrome_SubSequence(x,y-1),Longest_Palindrome_SubSequence(x+1,y));
			}
		}
		m[string(x,y+1)]=r;
		return r;
	}

	///@brief bottom up method - not ready yet!
	int Longest_Palindrome_SubSequence(const string& s){
		int sz=s.size();
		
		const char* p=&s.at(0);
		const char* q=&s.at(s.size()-1);

		int LP_size=1;
		char* LP_head=NULL;
		char* LP_tail=NULL;
		//int LP_size=LP_tail-LP_head+1;

		char lastchar='\0';
		bool firstmatch=true;
		for(int i=1;i<=sz;i++){
			char newchar=p[i];
			if(IsContainChar(p,p+i-1,newchar)){
				bool validmatch=false;
				if (firstmatch){
					if (newchar==*(p+i-1)){LP_size=2;}else{LP_size=3;}
					firstmatch=false;
					validmatch=true;
				}else{
					// if the char order is correct
					for (int j=0;j<=i;j++){
						if (p[j]==lastchar){break;}
						if (p[j]==newchar){validmatch=true;break;}
					}
					if (validmatch){
						LP_size+=2;
					}
				}
				lastchar=newchar;//??
			}
		}
		return LP_size;
	}



	}
	

	//////////////////////////////////////////////////////////////////////////
	class Solution1{/// O(n^2), best O(n)
	public:
		map<string, int> m;//???
		int lengthOfLongestSubstring(string s) {/// O(n^2)
			// Start typing your C/C++ solution below
			// DO NOT write int main() function
			if(s.empty()) 
				return 0;
			else
				return helper(&s.at(0),&s.back());

		}

		///@brief helper function for lengthOfLongestSubstring(string s)
		int helper(char* head, char* tail){
			int sz=tail-head+1;
			if(sz<=1)
				return 1;
			else{
				char* newtail=tail -1;
				if (*tail==*newtail){
					return helper(head,newtail);/// 
				}else{
					return mymax(helper(head,newtail),getfromright(head,newtail,*tail)); /// O(n) + ...
				}
			}    
		}

		int getfromright(char* head,char* newtail,char c){
			if(head==newtail)return 2;
			char* tmp=newtail-1;
			int r=2;
			unordered_set<char> se;
			//stdext::hash_set<int> hs;
			se.insert(c);
			se.insert(*newtail);
			while(tmp>=head){///O(n)
				if(se.find(*tmp)==se.end()){
					se.insert(*tmp);
					r++;
				}else{
					break;
				}
				tmp--;
			}
			cout<< newtail-tmp << "\t";
			copy(tmp,newtail+1,ostream_iterator<char>(cout,""));cout<<c<<endl;
			return r;
		}

		///@brief most efficient
		int lengthOfLongestSubstring_best(string s) {/// O(n)
			int n = s.length();
			int i = 0, j = 0;
			int maxLen = 0;
			bool exist[256] = { false };
			while (j < n) {
				if (exist[s[j]]) {
					maxLen = mymax(maxLen, j-i);
					while (s[i] != s[j]) {
						exist[s[i]] = false;
						i++;
					}
					i++;
					j++;
				} else {
					exist[s[j]] = true;
					j++;
				}
			}
			maxLen = mymax(maxLen, n-i);
			return maxLen;
		}

		///@brief Unordered map is an associative container that contains key-value pairs with unique keys.
		///Search, insertion, and removal have average constant-time complexity. 
		int lengthOfLongestSubstring_myversion(string s) {
			if (s.empty()){return 0;}
			unordered_map<char,int> uomap;
			int globalmax=0, localmax=0;
			for (int i=0;i<=s.size()-1;i++){
				if (uomap.find(s[i])==uomap.end()){
					printf("%c",s[i]);
					localmax++;
				}else{
					i=uomap[s[i]]+1;
					uomap.clear();//wrong!
					globalmax=mymax(globalmax,localmax);
					if (globalmax+i>s.size()){break;}
					localmax=1;
					printf("\n%c",s[i]);
				}
				uomap[s[i]]=i;
			}
			globalmax=mymax(globalmax,localmax);
			printf("\n");
			return globalmax;
		}
	};

	void test(){
        string sp="bcbaab";//"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";//"ababacdcab";//"eegiicgaeadbcfacfhifdbiehbgejcaeggcgbahfcajfhjjdgj";
        //int i=Palindrome::minCut(sp);
        int j=Palindrome::minCut2(sp);
        int k=Palindrome::minCut3(sp);
        cout << DP::Palindrome::str2mc.size() << endl;
		/*int LP_sz1=Longest_Palindrome("character");
		cout << LP_sz1<< endl;
		int LP_sz2=Longest_Palindrome("charcater");
		cout << LP_sz2<< endl;
		int LP_sz3=Longest_Palindrome("charcatera"); // carac, cacac - 5
		*/

		/*Run Status: Accepted!
		Program Runtime: 308 milli secs
		Progress: 1001/1001 test cases passed.*/
		Solution1 slu;
		slu.lengthOfLongestSubstring("abcddcbae");
		string //ss("wlrbbmqhcbdarzowkk");
		ss("hchzvfrkmlnozjk");
		cout<< slu.lengthOfLongestSubstring_best(ss) << endl;
		cout<< slu.lengthOfLongestSubstring_myversion(ss) << endl;
		cout<< slu.lengthOfLongestSubstring(ss) << endl;

		string s="character";
		int ls=Palindrome::Longest_Palindrome_SubSequence(&s.at(0),&s.at(s.size()-1));
		cout << ls << endl;
	}
	


}