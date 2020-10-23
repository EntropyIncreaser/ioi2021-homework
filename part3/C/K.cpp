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

const int N = 110;

int n, m;
int a[N];
int cnt[N * N];
bool dp[N * N][N * 2];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  int lim = min(a[1], a[n]);
  for (int i = 1; i <= n; ++i) {
    a[i] += a[i - 1];
    ++cnt[a[i] + 1];
  }
  m = a[n];
  for (int i = 1; i <= m; ++i) {
    cnt[i] += cnt[i - 1];
  }
  int ans = 0;
  for (int k = lim; k; --k) {
    memset(dp, 0, sizeof(dp));
    dp[k][(k - 1) * 2] = true;
    for (int i = k; i <= m - k; ++i) {
      dp[i + 1][1] |= dp[i][0];
      dp[i + 1][0] |= dp[i][1] | dp[i][2];
      for (int j = 2; j <= k; ++j) {
        if (cnt[i + j] != cnt[i + 1]) {
          break;
        }
        dp[i + j][(j - 1) * 2] |= dp[i][j * 2] | dp[i][j * 2 - 1];
        dp[i + j][j * 2 - 1] |= dp[i][j * 2 - 3];
      }
    }
    if (dp[m - k][(k - 1) * 2 - 1] || k == 1 && m == 2) {
      ans = k;
      break;
    }
  }
  if (ans) {
    cout << ans << '\n';
  } else {
    cout << "no quotation\n";
  }


#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
