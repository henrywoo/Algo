#ifndef NUMOFPOINTINLINEIN2DPLANE
#define NUMOFPOINTINLINEIN2DPLANE

#include <vector>
#include <map>
#include <algorithm>

namespace _NUMOFPOINTINLINEIN2DPLANE{
  using namespace std;

  struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
  };

  int maxPoints(vector<Point> &points) {
    if (points.empty()) return 0;
    size_t sz0 = points.size();
    if (sz0 <= 2) return points.size();
    // find duplicated points
    using DMAP = unordered_map<int, unordered_map<int, int>>;
    using DMAP2 = vector<vector<int>>;
    DMAP dm;
    vector<Point> points_;
    for (Point& p : points){
      dm[p.x][p.y]++;
      if (dm[p.x][p.y] == 1) points_.push_back(p);
    }
    points = points_;

    auto f = [&dm](Point& p, int& i){
      int t = dm[p.x][p.y];
      if (t > 1){
        i += t - 1;
      }
    };

    int gmax = 0;
    int sz = points.size();
    if (sz == 1){
      return dm[points[0].x][points[0].y];
    }
    vector<int> t(sz);
    DMAP2 d2 = DMAP2(sz, t);
    for (int i = 0; i < sz; ++i){
      Point& p1 = points[i];
      map<double, int> gradient2count;
      int verticalpoints = 0;
      for (int j = 0; j < sz; ++j){
        //if(gmax>=lmax+(sz-j)){break;}
        if (i == j){ continue; }
        if (d2[i][j]){ continue; }
        d2[j][i] = 1;
        Point& p2 = points[j];
        
        int xdiff = p2.x - p1.x;
        if (xdiff == 0){
          f(p2, verticalpoints);
          if (verticalpoints == 0){
            verticalpoints = 2;
          }else{
            verticalpoints++;
          }
          f(p2, verticalpoints);
          continue;
        }
        double gradient = (p2.y - p1.y)*1.0 / xdiff;
        printf("%d,%d -> %d,%d(%d)\t", p1.x, p1.y, p2.x, p2.y, dm[p2.x][p2.y]);
        printf("%f\n", gradient);
        if (gradient2count.find(gradient) == gradient2count.end()){
          gradient2count[gradient] = 2;
        }else{
          gradient2count[gradient]++;
        }
        f(p2, gradient2count[gradient]);
      }

      int lmax = 0;
      for (auto& pr : gradient2count){
        if (pr.second > lmax) lmax = pr.second;
      }
      lmax = max(lmax, verticalpoints);
      f(p1, lmax);
      printf("lmax=%d\n", lmax);
      gmax = max(lmax, gmax);
      if (gmax == sz0) return gmax;
    }
    return gmax;
  }

  bool test(){
    vector<Point> vp = { { 0, -12 }, { 5, 2 }, { 2, 5 }, { 0, -5 }, { 1, 5 }, { 2, -2 }, { 5, -4 }, { 3, 4 }, { -2, 4 }, { -1, 4 }, { 0, -5 }, { 0, -8 }, { -2, -1 }, { 0, -11 }, { 0, -9 }};
    printf("maxPoints:%d\n", maxPoints(vp));
    return true;
  }
}

#endif