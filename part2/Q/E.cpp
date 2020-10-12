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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("equal.in", "r", stdin);
  freopen("equal.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;
  int A = *max_element(a.begin(), a.end());
  vector<int> vis(A + 1);
  for (int x : a) ++vis[x];
  vector<int> profit;
  for (int x = 1; x <= A; ++x)
    if (vis[x]) {
      bool f = false;
      for (int y = x + x; y <= A; y += x) f |= vis[y];
      if (f) profit.push_back(vis[x]);
    }
  sort(profit.begin(), profit.end());
  vector<int> ans1(n + 1), ans2(n + 1);
  ans1[0] = ans2[0] = vis.size() - count(vis.begin(), vis.end(), 0);
  int pre = 0;
  for (int x : profit) {
    pre += x;
    --ans1[pre];
  }
  profit.clear();
  pre = 0;
  for (int x = 1; x <= A; ++x) if (vis[x]) profit.push_back(vis[x]);
  sort(profit.begin(), profit.end());
  for (int x : profit) {
    pre += x;
    --ans2[pre];
  }
  for (int i = 1; i <= n; ++i) ans1[i] += ans1[i - 1];
  for (int i = 1; i <= n; ++i) ans2[i] += ans2[i - 1];
  vector<int> ans(n + 1);
  for (int i = 0; i <= n; ++i) ans[i] = min(ans1[i], ans2[i] + 1);
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
