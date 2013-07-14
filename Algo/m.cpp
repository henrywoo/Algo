#include <iostream>
#include <iterator>
#include "bst.h"

using namespace std;

int main(int argc,char* argv[]){
	cout << "hello" << endl;
	int a[]={10,5,7,6,40,25,50,13,21,16,19,9,23,8};
	//int a[]={10,7,20,30,15,14};
	bst t;
	t.insert(a,sizeof(a)/sizeof(int));

	/*
	10 5 7 6 9 8 40 25 13 21 16 19 23 50
	5 6 7 8 9 10 13 16 19 21 23 25 40 50
	6 8 9 7 5 19 16 23 21 13 25 50 40 10
	*/

	vector<int> v=t.walk(PREORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;

	v=t.walk(INORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	
	v=t.walk(POSTORDER);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));

	//int i=-0;
	//cin >> i;
	return 0;
}