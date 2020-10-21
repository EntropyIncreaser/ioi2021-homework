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

const int N = 100010;

int n, m, k;
int in[N];
vector<int> G[N];

int ans[N];
vector<pair<int, int>> ed;

set<int> d0, d1;

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("graph.in", "r", stdin);
  freopen("graph.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> k;
  while (m--) {
    int u, v;
    cin >> u >> v;
    G[u].push_back(v);
    ++in[v];
  }
  for (int i = 1; i <= n; ++i) {
    if (in[i] == 0) {
      d0.insert(i);
    }
  }
  for (int i = 1; i <= n; ++i) {
    while (k && d0.size() > 1) {
      d1.insert(*d0.begin());
      d0.erase(d0.begin());
      --k;
    }
    if (k && d0.size() == 1 && !d1.empty() && *d0.begin() < *d1.rbegin()) {
      --k;
      d1.insert(*d0.begin());
      d0.clear();
    }
    int u;
    if (d0.empty()) {
      u = *d1.rbegin();
      d1.erase(prev(d1.end()));
      ed.emplace_back(ans[i - 1], u);
    } else {
      u = *d0.begin();
      d0.erase(d0.begin());
    }
    ans[i] = u;
    for (int v : G[u]) {
      if (--in[v] == 0) {
        d0.insert(v);
      }
    }
  }
  cout << vector<int>(ans + 1, ans + n + 1) << '\n';
  cout << ed.size() << '\n';
  for (auto[u, v] : ed) {
    cout << u << ' ' << v << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
