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

template<class T>
void chkMax(T &x, const T &y) {
  if (x < y) x = y;
}

template<class T>
void chkMin(T &x, const T &y) {
  if (x > y) x = y;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> a(n), b(n), d(n);
    vector<int> dis(n * 2);
    for (int i = 0; i < n; ++i) cin >> a[i] >> b[i] >> d[i];
    for (int i = 0; i < n; ++i) {
      dis[i] = a[i];
      dis[i + n] = b[i];
    }
    sort(dis.begin(), dis.end());
    for (int i = 0; i < n; ++i) {
      a[i] = lower_bound(dis.begin(), dis.end(), a[i]) - dis.begin();
      b[i] = lower_bound(dis.begin(), dis.end(), b[i]) - dis.begin();
    }
    vector<vector<pair<int, int>>> segs(n * 2, vector<pair<int, int>>(n * 2));
    for (int i = 0; i < n; ++i)
      chkMax(segs[a[i]][b[i]], make_pair(d[i], i));
    for (int len = 1; len < n * 2; ++len)
      for (int i = 0, j = len; j < n * 2; ++i, ++j)
        segs[i][j] = max({segs[i][j], segs[i + 1][j], segs[i][j - 1]});
    vector<vector<int>> dp(n * 2, vector<int>(n * 2, -1));
    function<int(int, int)> dfs = [&](int l, int r) {
      if (l > r)return 0;
      if (dp[l][r] != -1)
        return dp[l][r];
      if (segs[l][r].first == 0)dp[l][r] = 0;
      else {
        dp[l][r] = numeric_limits<int>::max();
        int id = segs[l][r].second;
        for (int i = a[id]; i <= b[id]; ++i)
          chkMin(dp[l][r], dfs(l, i - 1) + dfs(i + 1, r));
        dp[l][r] += segs[l][r].first;
      }
      return dp[l][r];
    };
    cout << dfs(0, n * 2 - 1) << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
