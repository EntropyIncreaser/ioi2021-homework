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

const int N = 300010;

int n, m;
int f[N], deg[N], d2[N], sz[N];
tuple<int, int, int> ed[N];
int key[N];

int curx, curv;

int find(int x) { return f[x] == x ? x : f[x] = find(f[x]); }

void ment(int x) {
  if (!f[x]) {
    f[x] = x;
    sz[x] = 1;
    ++curv;
  }
}

void contr(int x, int sgn) {
  curx += d2[x] * sgn;
  if (d2[x] == sz[x])
    curx -= sgn;
}

int x[N], y[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 1; i <= m; ++i) {
    int a, b, p;
    cin >> a >> b >> p;
    ed[i] = make_tuple(p, a, b);
  }
  sort(ed + 1, ed + m + 1, greater<tuple<int, int, int>>());
  for (int i = 1; i <= m; ++i) {
    int a, b;
    tie(key[i], a, b) = ed[i];
    ment(a);
    ment(b);
    int fa = find(a), fb = find(b);
    contr(fa, -1);
    if (fa != fb)contr(fb, -1);
    if (deg[a]++ == 2)--d2[fa];
    else if (deg[a] == 2)++d2[fa];
    if (deg[b]++ == 2)--d2[fb];
    else if (deg[b] == 2)++d2[fb];
    if (fa != fb) {
      f[fa] = fb;
      sz[fb] += sz[fa];
      d2[fb] += d2[fa];
    }
    contr(fb, 1);
    x[i] = curv - curx;
    y[i] = i - curx;
  }
  int q;
  cin >> q;
  while (q--) {
    int t;
    cin >> t;
    int id = upper_bound(key + 1, key + m + 1, t, greater<int>()) - key - 1;
    cout << x[id] << ' ' << y[id] << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
