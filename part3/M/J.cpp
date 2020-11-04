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

const ll INF = 1LL << 50;

const int N = 300010;

int a[N];
map<pair<int, int>, vector<int>> seq;
map<pair<int, int>, int> cost[4];

ll gval(const map<pair<int, int>, int> &mp, const pair<int, int> &key) {
  auto it = mp.find(key);
  if (it == mp.end()) return INF;
  return it->second;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, d;
  cin >> n >> d;
  for (int i = 1; i <= d; ++i)cin >> a[i];
  for (int i = 1; i < d; ++i) {
    int x = a[i], y = a[i + 1], f = 0;
    if (x > y) {
      swap(x, y);
      f = 1;
    }
    seq[make_pair(x, y)].push_back(f);
  }
  int m;
  cin >> m;
  while (m--) {
    int u, v, p;
    char t;
    cin >> u >> v >> t >> p;
    int typ = (t == 'R') << 1;
    if (u > v) {
      swap(u, v);
      typ |= 1;
    }
    auto it = cost[typ].find(make_pair(u, v));
    if (it == cost[typ].end())cost[typ].insert(make_pair(make_pair(u, v), p));
    else it->second = min(it->second, p);
  }
  ll ans = 0;
  for (const auto &pr : seq) {
    auto se = pr.second;
    static ll f[4];
    for (int i = 0; i < 4; ++i) f[i] = gval(cost[i], pr.first);
    f[0] = min(f[0], f[2]);
    f[1] = min(f[1], f[3]);
    f[2] = min(0LL, f[2] - f[0] - f[1]);
    f[3] = min(0LL, f[3] - f[0] - f[1]);
    int c0 = 0, c1 = 0;
    for (int i = 0; i < se.size(); ++i) {
      ans += f[se[i]];
      if (se[i] == 0) ++c0;
      else ++c1;
    }
    c0 = min(c0, c1);
    int sel = 0, v = 0;
    if (f[2] < f[3]) {
      for (int i = 0; i < se.size(); ++i)
        if (se[i] == 0) ++v;
        else if (v) {
          --v;
          ++sel;
        }
      ans += f[2] * sel + f[3] * (c0 - sel);
    } else {
      for (int i = 0; i < se.size(); ++i)
        if (se[i] == 1) ++v;
        else if (v) {
          --v;
          ++sel;
        }
      ans += f[3] * sel + f[2] * (c0 - sel);
    }
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
