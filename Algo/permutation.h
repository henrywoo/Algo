#ifndef __ALGO_PERMUTATION__
#define __ALGO_PERMUTATION__
#include <iostream>

namespace recursion{
  void swap(char* x, char* y){
    if (x != y){
      *x ^= *y;
      *y ^= *x;
      *x ^= *y;
    }
  }

  // use vector to store stack
  // 有点像DFS
  void permute(char* p){
    static vector<char> vc;
    static auto f = [&](){for (char c : vc)cout << c; cout << "\n";};
    char* p2 = p;
    if (*p == 0){f();}

    while (*p2){
      swap(p, p2);
      vc.push_back(p[0]);
      permute(p+1);
      vc.pop_back();
      swap(p, p2);
      ++p2;
    }
  }

  void permute(char *a, int i, int n){
    int j;
    if (i == n)
      printf("%s\n", a);
    else{
      for (j = i; j <= n; j++){
        swap((a + i), (a + j));
        permute(a, i + 1, n);
        swap((a + i), (a + j)); //backtrack
      }
    }
  }


}




#endif