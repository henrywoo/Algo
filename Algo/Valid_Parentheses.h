#ifndef __ALGO_VALID_PARENTHESES__
#define __ALGO_VALID_PARENTHESES__

#include <stack>

namespace stackapplication{

    bool IsAPair(char x,char y){
        return ((x=='[' && y==']') ||
            (x=='(' && y==')') ||
            (x=='{' && y=='}')
            );
    }

    bool IsValidParentheses(const string& s){
        std::stack<char> stk;
        for (int i=0;i<s.size();++i){
            if (stk.empty()){
                stk.push(s[i]);
            }else{
                if (IsAPair(stk.top(),s[i])){///
                    stk.pop();
                    continue;
                }else{
                    stk.push(s[i]);
                }
            }
        }
        return stk.empty();
    }

    


}

#endif