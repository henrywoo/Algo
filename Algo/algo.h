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

namespace Augmentation{

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
		set<T>::const_iterator sei=se.end();
		--sei;
		return *sei;
	}


#define ARRAYSIZE(x,t) (sizeof(x)/sizeof(t))
	///@brief Maximums in Sliding Windows
	///Time Complexity -> All cases: O(N*logM)
	///[N-size of the big array; M-size of the window]
	template<class T>
	vector<T> MaxInSlidingWindows(T* head, T* tail, int sz){
		vector<T> v;
		set<T> se;
		
		// initial setup - O(M)
		for (int i=0;i<=sz-1;i++){
			se.insert(head[i]);
		}
		v.push_back(GetMaxOfSet(se));

		// (N-M)*2*logM
		T* tmppush=head+sz;// the item after the sz'th item
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
		vector<int> v=MaxInSlidingWindows<int>(a,a+ARRAYSIZE(a,int)-1,1);
		copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));cout << endl;

		int b[]={9,7,8,6,5,4,3,2,1};
		vector<int> ip(b,b+ARRAYSIZE(b,int)-1);
		v=maxInWindows(ip, 1);//requires size of SD>1
		copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));cout << endl;
		return true;
	}
	
}

namespace DP{

	using namespace std;

	/// substring -consecutive; subsequence - not necessarily consecutive
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
	  = max(longest(i+1,j),longest(i,j-1))
	************************************************************************/
    #define MAX(x,y) (x>y?x:y)
	///@brief top down method
	int Longest_Palindrome(const char* x,const char*y){
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
				r=2+Longest_Palindrome(x+1,y-1);
			}else{
				r=MAX(Longest_Palindrome(x,y-1),Longest_Palindrome(x+1,y));
			}
		}
		m[string(x,y+1)]=r;
		return r;
	}

	///@brief bottom up method - not ready yet!
	int Longest_Palindrome(const string& s){
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
					return std::max(helper(head,newtail),getfromright(head,newtail,*tail)); /// O(n) + ...
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
					maxLen = max(maxLen, j-i);
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
			maxLen = max(maxLen, n-i);
			return maxLen;
		}

		///@brief Unordered map is an associative container that contains key-value pairs with unique keys.
		///Search, insertion, and removal have average constant-time complexity. 
		int lengthOfLongestSubstring2(string s) {
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
					globalmax=std::max(globalmax,localmax);
					if (globalmax+i>s.size()){break;}
					localmax=1;
					printf("\n%c",s[i]);
				}
				uomap[s[i]]=i;
			}
			globalmax=std::max(globalmax,localmax);
			printf("\n");
			return globalmax;
		}
	};

	void test(){
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
		cout<< slu.lengthOfLongestSubstring2(ss) << endl;
		//reverse(ss.begin(),ss.end());
		//cout<< slu.lengthOfLongestSubstring2(ss) << endl;
		cout<< slu.lengthOfLongestSubstring(ss) << endl;

		string s="character";
		int ls=Longest_Palindrome(&s.at(0),&s.at(s.size()-1));
		cout << ls << endl;
	}
	


}