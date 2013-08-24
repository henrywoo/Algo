#include <vector>
/*
The language PigEwu has a very simple syntax. Each word in this language has exactly 4 letters. Also each word contains exactly two vowels (y is consider a vowel in PigEwu). For instance, "maar" and "even" are legitimate words, "arts" is not a legal word.

In the game boggle, you are given a 4x4 array of letters and asked to find all words contained in it. A word in our case (PigEwu) will thus be a sequence of 4 distinct squares (letters) that form a legal word and such that each square touches (have a corner or edge in common) the next square.

For example:

A  S  S  D 
S  B  E  Y
G  F  O  I
H  U  U  K

In this board a (partial) list of legal words include:

ASGU    SABO    FOIK    FOYD    SYDE    HUFO

BEBO is a legal word but it is not on this boggle board (there are no two B's here).

Write a program that reads a pair of Boggle boards and lists all PigEwu words that are common to both boards.

Input 
The input file will include a few data sets. Each data set will be a pair of boards as shown in the sample input. All entries will be upper case letters. Two consecutive entries on same board will be separated by one blank. The first row in the first board will be on the same line as the first row of the second board. They will be separated by four spaces, the same will hold for the remaining 3 rows. Board pairs will be separated by a blank line. The file will be terminated by `#'.

Output 
For each pair of boggle boards, output an alphabetically-sorted list of all common words, each word on a separate line; or the statement "There are no common words for this pair of boggle boards."

Separate the output for each pair of boggle boards with a blank line.

Sample Input 

D F F B    W A S U
T U G I    B R E T
O K J M    Y A P Q
K M B E    L O Y R

Z W A V    G S F U
U N C O    A H F T
Y T G I    G N A L
H G P M    B O O B

#

Sample Output 

There are no common words for this pair of boggle boards.

ANGO
AOGN
GNAO
GOAN
NAOG
NGOA
OANG
OGNA

*/
#include <map>
#include <math.h>

namespace boggle{
    using namespace std;

    void gen_char2index(vector<string>& vvc,
        multimap<char,unsigned int>& char2index,
        map<unsigned int,char>& index2char)
    {
        int sz=vvc.size();//vector size - nrow
        int index=0;
        for (int i=0;i<sz;++i){
            int sz2=vvc[i].size();//string size - ncol
            for (int j=0;j<sz2;++j){
                char2index.insert(pair<char,unsigned int>(vvc[i][j],index));
                index2char[index]=vvc[i][j];
                index++;
            }
        }
    }

    int corr2index(pair<int,int>& p,int ncol){
        return p.first*ncol + p.second;
    }

    int corr2index(int i, int j, int ncol){
        return i*ncol + j;
    }


    void fillmatrixfornode(int index,vector<vector<bool>>& vvb){
        int sz=sqrt(vvb.size());//nrow=ncol=sz
        int rowid=index/sz;
        int colid=index - index/sz*sz;

        if (rowid-1>=0){
            vvb[corr2index(rowid,colid,sz)][corr2index(rowid-1,colid,sz)]=true;
            vvb[corr2index(rowid-1,colid,sz)][corr2index(rowid,colid,sz)]=true;
            if (colid-1>=0){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid-1,colid-1,sz)]=true;
                vvb[corr2index(rowid-1,colid-1,sz)][corr2index(rowid,colid,sz)]=true;
            }
            if (colid+1<sz){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid-1,colid+1,sz)]=true;
                vvb[corr2index(rowid-1,colid+1,sz)][corr2index(rowid,colid,sz)]=true;
            }
        }
        if (colid-1>=0){
            vvb[corr2index(rowid,colid,sz)][corr2index(rowid,colid-1,sz)]=true;
            vvb[corr2index(rowid,colid-1,sz)][corr2index(rowid,colid,sz)]=true;
            if (rowid+1<sz){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid+1,colid-1,sz)]=true;
                vvb[corr2index(rowid+1,colid-1,sz)][corr2index(rowid,colid,sz)]=true;
            }
        }
        if (rowid+1<sz){
            int x=corr2index(rowid,colid,sz);
            int y=corr2index(rowid+1,colid,sz);
            vvb[x][y]=true;
            vvb[corr2index(rowid+1,colid,sz)][corr2index(rowid,colid,sz)]=true;
            if (colid-1>=0){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid+1,colid-1,sz)]=true;
                vvb[corr2index(rowid+1,colid-1,sz)][corr2index(rowid,colid,sz)]=true;
            }
            if (colid+1<sz){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid+1,colid+1,sz)]=true;
                vvb[corr2index(rowid+1,colid+1,sz)][corr2index(rowid,colid,sz)]=true;
            }
        }
        if (colid+1<sz){
            vvb[corr2index(rowid,colid,sz)][corr2index(rowid,colid+1,sz)]=true;
            vvb[corr2index(rowid,colid+1,sz)][corr2index(rowid,colid,sz)]=true;
            if (rowid-1>=0){
                vvb[corr2index(rowid,colid,sz)][corr2index(rowid-1,colid+1,sz)]=true;
                vvb[corr2index(rowid-1,colid+1,sz)][corr2index(rowid,colid,sz)]=true;
            }
        }

    }

    vector<vector<int>> mycombination(vector<set<int>> vsi){
        vector<vector<int>> vvi;
        if (!vsi.empty()){
            if(vsi.size()==1){
                set<int>& si=vsi[0];
                for (set<int>::iterator i=si.begin();i!=si.end();++i){
                    vvi.push_back(vector<int>(1,*i));
                }
            }else{
                vector<set<int>> vsi1(vsi.begin(),vsi.end()-1);
                vector<vector<int>> vvi1=mycombination(vsi1);
                set<int>& si=vsi.back();
                for (set<int>::iterator i=si.begin();i!=si.end();++i){
                    for (int j=0;j<vvi1.size();++j){
                        vector<int> vi=vvi1[j];
                        if (find(vi.begin(),vi.end(),*i)==vi.end()){///
                            vi.push_back(*i);
                            vvi.push_back(vi);
                        }else{
                            //cout << "."<<*i<<".";
                        }
                    }
                }
            }
        }
        return vvi;
    }


    vector<vector<int>> bogglesolver(vector<string>& vvc, string& target){
        vector<vector<int>> result;
        //binary matrix
        int sz=vvc.size();
        vector<bool> vb(sz*sz,false);
        vector<vector<bool>> udagmatrix(sz*sz,vb);
        
        //mapping
        multimap<char,unsigned int> char2index;//very useful
        map<unsigned int,char> index2char;
        gen_char2index(vvc,char2index,index2char);

        //configuration graph
        //int sz=vvc.size();//vector size - nrow
        int index=0;
        for (int i=0;i<sz;++i){
            int sz2=vvc[i].size();//string size - ncol
            for (int j=0;j<sz2;++j){
                fillmatrixfornode(index++,udagmatrix);
            }
        }

#ifdef _DEBUG
        for (int i=0;i<udagmatrix.size();++i){
            for (int j=0;j<udagmatrix[i].size();++j){
                cout<< udagmatrix[i][j] << " ";
            }
            cout << endl;
        }
#endif

        vector<set<int>> vsi;
        typedef multimap<char,unsigned int>::const_iterator cit;
        for (int i=0;i<target.size();++i){
            pair<cit,cit> pcc=char2index.equal_range(target[i]);
            cit tmp1=pcc.first;
            set<int> tmpsi;
            while(tmp1!=pcc.second){
                int ind=tmp1->second;//9
                tmpsi.insert(ind);
                ++tmp1;
            }
            vsi.push_back(tmpsi);
        }

        // equivalent to find the combination of the four groups of ints
        vector<vector<int>> vvi=mycombination(vsi);
        for (int i=0;i<vvi.size();i++){
            for (int j=0;j<vvi[i].size();j++){
                cout << vvi[i][j] << " ";
            }
            cout << endl;
        }

        for (int i=0;i<vvi.size();i++){
            if (!vvi[i].empty()){
                int tmp=vvi[i][0];
                bool bingo=true;
                for (int j=1;j<vvi[i].size();j++){
                    //check vvb[][]==true
                    if (!udagmatrix[tmp][vvi[i][j]]){
                        //we can return here
                        bingo=false;
                        break;
                    }
                    tmp=vvi[i][j];
                }
                if(bingo){
                    result.push_back(vvi[i]);
                    cout << "bingo!:";
                    copy(vvi[i].begin(),vvi[i].end(),ostream_iterator<int>(cout," "));
                    cout << endl;
                }
            }
        }
        return result;
    }

    void test(){
        vector<string> vvc; //vector<vector<char>> vvc;
        vvc.push_back("ASSDS");
        vvc.push_back("SBEYB");
        vvc.push_back("GFOIF");
        vvc.push_back("HUUKA");
        vvc.push_back("SYEMD");

        //string target("SSEF");
        vector<string> vs(1,string("ASGU"));
        vs.push_back("SABO");
        vs.push_back("FOIK");
        vs.push_back("FOYD");
        vs.push_back("SYDE");
        vs.push_back("HUFO");

        for (int i=0;i<vs.size();i++){
            bogglesolver(vvc,vs[i]);
        }
    }



}
