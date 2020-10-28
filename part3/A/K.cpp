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

const int N = 1000010, L = 21;

int n, k;
int f[N * 2][L];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k;
  for (int i = 1; i <= n * 2; ++i)f[i][0] = i;
  while (k--) {
    int l, r;
    cin >> l >> r;
    if (l <= r) {
      f[l][0] = max(f[l][0], r + 1);
      f[l + n][0] = max(f[l + n][0], r + n + 1);
    } else {
      f[1][0] = max(f[1][0], r + 1);
      f[l][0] = max(f[l][0], r + n + 1);
      f[n + l][0] = max(f[n + l][0], n * 2 + 1);
    }
  }
  fill_n(f[n * 2 + 1], L, n * 2 + 1);
  for (int i = 1; i <= n * 2; ++i) {
    f[i][0] = max(f[i][0], f[i - 1][0]);
  }
  for (int i = n * 2; i; --i)
    for (int j = 1; j < L; ++j)
      f[i][j] = f[f[i][j - 1]][j - 1];
  int ans = numeric_limits<int>::max();
  for (int i = 1; i <= n; ++i) {
    int cur = 0, x = i;
    for (int j = L - 1; j >= 0; --j)
      if (f[x][j] < i + n) {
        cur |= 1 << j;
        x = f[x][j];
      }
    if (f[x][0] >= i + n)
      ans = min(ans, cur + 1);
  }
  if (ans == numeric_limits<int>::max())
    cout << "impossible\n";
  else
    cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
