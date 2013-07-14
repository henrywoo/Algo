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
	vector<int> v=t.walk(INORDER,true);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	v=t.walk(PREORDER,false);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout << endl;
	v=t.walk(POSTORDER,false);
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));

	//int i=-0;
	//cin >> i;
	return 0;
}