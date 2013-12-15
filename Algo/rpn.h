#ifndef __ALGO_RPN__
#define __ALGO_RPN__
#include <string>
#include <vector>
#include <stack>

namespace RPN{
  using namespace std;
  //RPN (Reverse Polish Notation) is a mathematical notation wherein every operator
  //follows all of its operands, which was designed to reduce computer memory access
  //and utilize the stack to evaluate expressions.

  bool isValidRPN(const string& s){
    return true;
  }

  int SolveRPN(const string& s){
    return true;
  }


  class Solution {
  public:
    class binop{
      stack<int> stk;
      //int result=0;
      //size_t st=0;

      void _put(int i){
        //if(st>=2){throw std::invalid_argument("invalid input string");}
        //buf[st++]=i;
        stk.push(i);
      }

      void _put(const char* c){
        int t2 = stk.top(); stk.pop();
        int t1 = stk.top(); stk.pop();
        int tmp = 0;
        switch (*c){
        case '+': tmp = t1 + t2; break;
        case '-': tmp = t1 - t2; break;
        case '*': tmp = t1*t2; break;
        case '/': tmp = t1 / t2; break;
        }
        stk.push(tmp);
      }
    public:
      void put(const string& s){
        if (s == "+" || s == "-" || s == "*" || s == "/"){
          _put(s.c_str());
        }
        else{
          int i = atoi(s.c_str());
          _put(i);
        }
      }
      int get(){
        return stk.top();
      }
    };

    int evalRPN(vector<string> &tokens) {
      binop bo;
      size_t sz = tokens.size();
      for (int i = 0; i < sz; ++i){
        bo.put(tokens[i]);
      }
      return bo.get();
    }

    static bool test(){
      vector<string> vs = { "3", "-4", "+" };
      Solution s;
      printf("%d\n", s.evalRPN(vs));
      return true;
    }
  };
}



#endif