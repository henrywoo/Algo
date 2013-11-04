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
  enum OB:char{ NORMAL = 0, COLD, DIZZY, OBSZ };

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
  pair<vector<ST>, PROB> viterbi(const vector<OB>& observations)
  {
    assert(ST::SZ == emission.size());
    assert(!emission.empty() && (OB::OBSZ == emission[0].size()));
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
          statesprob[j] = log(ST_initial_prob[j] * emission[j][obv]);
        }else{
          for (k = 0, ma=INT_MIN; k < ST::SZ; ++k){
            double tmp = (laststatesprob[k] + log(tm[k][j] * emission[j][obv]));
            //cout << laststatesprob[k] << "*" << tm[k][j] << "*" << emission[j][obv] << endl;
            if (ma < tmp)
              ma = tmp;
          }
          statesprob[j] = ma;
        }
      }
      getmax(ma, index);
      vst.push_back((ST)index);
    }
    return { vst, ma };
  }

  ///@brief calculate the posterior probability for each state at each time point
  vector<vector<PROB>> forward_backward(const vector<OB>& observations){
    size_t OBLen = observations.size();
    vector<vector<PROB>> ForwardProb(OBLen, vector<PROB>(ST::SZ)); // time, state
    vector<vector<PROB>> BckwardProb(OBLen, vector<PROB>(ST::SZ)); // time, state
    vector<vector<PROB>> PosteriorProb(OBLen, vector<PROB>(ST::SZ)); // time, state

    // forward algo
    for (int i = 0; i < OBLen; ++i) {
      OB obv = observations[i];
      for (int j = 0; j < ST::SZ; ++j){
        if (i == 0){
          ForwardProb[i][j] = ST_initial_prob[j] * emission[j][obv];
        }else{
          for (int k = 0; k < ST::SZ; ++k){
            ForwardProb[i][j] += ForwardProb[i - 1][k] * tm[k][j] * emission[j][obv];
            //cout << ForwardProb[i - 1][k] << "*" << tm[k][j] << "*" << emission[j][obv] << endl;
          }
        }
      }
    }
    // at time T
    PROB ProbOfObs_1 = 0;
    ProbOfObs_1=accumulate(ForwardProb.back().begin(), ForwardProb.back().end(), ProbOfObs_1);
    //cout << ProbOfObs_1 << endl;

    // backward algo
    for (int i = OBLen - 1; i >= 0; --i) { // observation i
      for (int j = 0; j < ST::SZ; ++j){ // current state j
        if (i == OBLen - 1){
          BckwardProb[i][j]=1;
        }else{
          OB obv = observations[i+1];
          for (int k = 0; k < ST::SZ; ++k){ // state
            BckwardProb[i][j] += tm[j][k] * emission[k][obv] * BckwardProb[i+1][k];
            //cout << tm[j][k] << "*" << emission[k][obv] << "*" <<BckwardProb[i+1][k]  << endl;
          }
        }
      }
    }
    // at time 1 - different from forward
    PROB ProbOfObs_2 = 0;
    vector<PROB>& b=BckwardProb.front();
    for (int i = 0; i < ST::SZ;++i){
      ProbOfObs_2 += b[i] * emission[i][observations.front()] * ST_initial_prob[i];
    }
    //cout << ProbOfObs_2 << endl;

    // posterior probability
    for (size_t i = 0; i < OBLen; i++){
      for (int j = 0; j < ST::SZ; ++j){
        PosteriorProb[i][j] = ForwardProb[i][j] * BckwardProb[i][j] / ProbOfObs_1;
      }
    }
    return PosteriorProb;
  }

  bool test(){
    vector<OB> observations = { OB::NORMAL, OB::COLD, OB::DIZZY };
    // , OB::N, OB::D, OB::N, OB::C, OB::D, OB::N, OB::D, OB::N, OB::N, OB::C, OB::C, OB::N};
    pair<vector<ST>, PROB> pvd = viterbi(observations);
    cout << "Most likely sequence:\n";
    for (ST tmp: pvd.first){
      switch (tmp){
      case hmm::H:
        cout << "HEALTHY -> ";
        break;
      case hmm::F:
        cout << "FEVER -> ";
        break;
      default:
        break;
      }
    }
    cout << "||\n";
    cout << "Probability: " << exp(pvd.second) << endl;

    vector<vector<PROB>> PosteriorProb = forward_backward(observations);
    cout << "Posterior probability given observation and hmm model parameters:" << endl;
    for (int i = 0; i < PosteriorProb.size();++i){
      cout << "Time " << i + 1 << ":\t";
      for (PROB p : PosteriorProb[i]){
        cout << p << "\t";
      }
      cout << endl;
    }
    return true;
  }

}

#endif