#ifndef __ALGO_HMM__
#define __ALGO_HMM__

#include <vector>
#include <assert.h>
#include <iostream>
#include <cmath>

// Hidden Markov Model
namespace hmm{
  using namespace std;
  typedef double PROB;

  // states: H - healthy, F - fever
  enum ST:char{ H = 0, F, SZ };
  enum class OB:char{ NORMAL = 0, COLD, DIZZY, SZ };

  vector<PROB> ST_initial_prob = {0.6,0.4};
  vector<vector<PROB>> tm = {
    { 0.7, 0.3 },
    { 0.4, 0.6 }
  };
  // from P(OB|ST)
  vector<vector<PROB>> emission = {
    {0.5,0.4,0.1},// state 0 -> observations
    {0.1,0.3,0.6}
  };

  /*struct hmmparam{
    vector<unsigned char> states;
    vector<unsigned char> obs;
    vector<PROB> ST_initial_prob;
    vector<vector<PROB>> tm;
    vector<vector<PROB>> emission;
  };*/

  // viterbi path and corresponding probability
  pair<vector<ST>, PROB> viterbi(vector<OB>& observations)
  {
    assert(ST::SZ == emission.size());
    assert(!emission.empty() && ((int)OB::SZ == emission[0].size()));
    assert(ST::SZ == tm.size());
    assert(!tm.empty() && ST::SZ == tm[0].size());

    vector<ST> vst;
    vector<PROB> statesprob(ST::SZ);
    vector<PROB> laststatesprob(statesprob);
    auto getmax = [&statesprob](PROB& ma, int& index){
      ma = INT_MIN;
      for (int i = 0; i < ST::SZ; ++i){
        if (ma < statesprob[i]){
          ma = statesprob[i];
          index = i;
        }
      }};
    PROB ma;
    int index, i, j, k;
    for (i = 0; i < observations.size(); ++i) {
      laststatesprob = statesprob;
      OB obv = observations[i];
      for (j = 0; j < ST::SZ; ++j){
        if (i == 0){
          statesprob[j] = log(ST_initial_prob[j] * emission[j][(int)obv]);
        }else{
          for (k = 0, ma=INT_MIN; k < ST::SZ; ++k){
            double tmp = (laststatesprob[k] + log(tm[k][j] * emission[j][(int)obv]));
#ifdef _DEBUG
            cout << laststatesprob[k] << "*" << 
              tm[k][j] << "*" << emission[j][(int)obv] << endl;
#endif
            if (ma < tmp)
              ma = tmp;
          }
          statesprob[j] = ma;
        }
      }
      getmax(ma, index);
      vst.push_back((ST)index);
    }

    return pair<vector<ST>, PROB>(vst, ma);
  }

  bool test(){
    vector<OB> observations = { OB::NORMAL, OB::COLD, OB::DIZZY };
    // , OB::N, OB::D, OB::N, OB::C, OB::D, OB::N, OB::D, OB::N, OB::N, OB::C, OB::C, OB::N};
    pair<vector<ST>, PROB> pvd = viterbi(observations);
    cout << exp(pvd.second) << endl;
    for (ST tmp: pvd.first){
      switch (tmp){
      case hmm::H:
        cout << "HEALTHY" << endl;
        break;
      case hmm::F:
        cout << "FEVER" << endl;
        break;
      default:
        break;
      }
    }
    return true;
  }

}

#endif