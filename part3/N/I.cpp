/*
_/_/_/_/    _/_/_/_/_/  _/_/_/
_/      _/      _/    _/      _/
_/      _/      _/    _/      _/
_/      _/      _/    _/      _/
_/      _/      _/    _/  _/  _/
_/      _/  _/  _/    _/    _/_/
_/_/_/_/      _/_/     _/_/_/_/_/

_/_/_/_/    _/    _/  _/      _/
_/      _/   _/  _/   _/_/  _/_/
_/      _/    _/_/    _/ _/_/ _/
_/      _/     _/     _/  _/  _/
_/      _/    _/_/    _/      _/
_/      _/   _/  _/   _/      _/
_/_/_/_/    _/    _/  _/      _/

_/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/
    _/         _/     _/
    _/         _/     _/
    _/         _/     _/_/_/_/
    _/         _/     _/
    _/         _/     _/
    _/     _/_/_/_/_/ _/_/_/_/_/

_/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/
    _/         _/     _/
    _/         _/     _/
    _/         _/     _/_/_/_/
    _/         _/     _/
    _/         _/     _/
    _/     _/_/_/_/_/ _/_/_/_/_/
*/
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <random>
#include <bitset>
#include <queue>
#include <functional>
#include <set>
#include <map>
#include <vector>
#include <chrono>
#include <iostream>
#include <limits>
#include <numeric>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

double EPS = 1e-6;

const int X = 510;

int r[X][X];
int vis[X][X];

int sq(int x) {
  return x * x;
}

double dist(int x1, int y1, int x2, int y2) {
  return sqrt(sq(x1 - x2) + sq(y1 - y2)) * 10. - EPS;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif

  int n;
  scanf("%d", &n);
  while (n--) {
    int x, y, r1;
    scanf("%d %d 0.%d", &x, &y, &r1);
    r[x][y] = r1;
  }
  int q;
  scanf("%d", &q);
  for (int rep = 1; rep <= q; ++rep) {
    int x1, y1, x2, y2, ans = 0;
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
    auto work = [&](int x, int y) {
      if (x < 0 || x >= X || y < 0 || y >= X || !r[x][y] || vis[x][y] == rep) {
        return;
      }
      int dx = x2 - x1, dy = y2 - y1;
      vis[x][y] = rep;
      int xd = x - x1, yd = y - y1;
      double dis;
      if (xd * dx + yd * dy < 0) {
        dis = dist(x, y, x1, y1);
      } else if (xd * dx + yd * dy > dx * dx + dy * dy) {
        dis = dist(x, y, x2, y2);
      } else {
        dis = abs((xd * dy - dx * yd) / sqrt(dx * dx + dy * dy)) * 10.;
      }
      ans += r[x][y] >= dis;
    };
    auto around = [&](int x, int y) {
      work(x - 2, y);
      work(x - 1, y - 1);
      work(x - 1, y);
      work(x - 1, y + 1);
      work(x, y - 2);
      work(x, y - 1);
      work(x, y);
      work(x, y + 1);
      work(x, y + 2);
      work(x + 1, y - 1);
      work(x + 1, y);
      work(x + 1, y + 1);
      work(x + 2, y);
    };
    if (x1 != x2) {
      if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
      }
      for (int i = x1 - 2; i <= x2 + 2; ++i) {
        int py = (i - x1) * (y2 - y1) / (x2 - x1) + y1;
        around(i, py);
      }
    }
    if (y1 != y2) {
      if (y1 > y2) {
        swap(x1, x2);
        swap(y1, y2);
      }
      for (int i = y1 - 2; i <= y2 + 2; ++i) {
        int px = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        around(px, i);
      }
    }
    printf("%d\n", ans);
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
