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

#define ctz __builtin_ctz
#define clz __builtin_clz

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

const int N = 1010, L = 13, K = (1 << L) + 5;

int n;
int a[N];
bitset<K> f[N], g[N], h[N];
bitset<K> mask[L + 1];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 0; i <= L; ++i)
    for (int j = 0; j < K; j += 1 << i)
      mask[i].set(j);
  h[0].set(0);

  int T;
  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    int sum = accumulate(a + 1, a + n + 1, 0);
    if (sum & (sum - 1)) {
      cout << "no\n";
      continue;
    }
    sum = 0;
    for (int i = 1; i <= n; ++i) {
      int t = ctz(a[i]);
      f[i] = ((h[i - 1] & mask[t]) << a[i]) | (h[i - 1] & (mask[t] << (sum % a[i])));
      sum += a[i];
      g[i].reset();
      for (int j = 0; j <= sum; ++j) {
        if (j == 0 || j == sum || clz(j) != clz(sum - j)) g[i][j] = g[i][j] || f[i][j];
        else {
          int l = 31 - clz(j);
          g[i][j + (1 << l)] = g[i][j + (1 << l)] || f[i][j];
        }
      }
      h[i].reset();
      for (int j = 0; j <= sum; ++j) {
        h[i][j] = h[i][j] || g[i][j];
        int q = 31 - clz(max(j, sum - j));
        if ((j >> q) & 1) h[i][j - (1 << q)] = h[i][j - (1 << q)] || g[i][j];
        if (((sum - j) >> q) & 1) h[i][j + (1 << q)] = h[i][j + (1 << q)] || g[i][j];
      }
    }
    if (!g[n][sum] && !g[n][0]) {
      cout << "no\n";
      continue;
    }
    int j = g[n][0] ? 0 : sum;
    string ans;
    for (int i = n; i; --i) {
      if (!g[i][j]) {
        int q = 31 - clz(max(j, sum - j));
        j ^= 1 << q;
      }
      if (!f[i][j]) {
        int l = 31 - clz(j);
        j -= 1 << (l - 1);
      }
      sum -= a[i];
      if (j >= a[i] && j % a[i] == 0 && h[i - 1][j - a[i]]) {
        j -= a[i];
        ans.push_back('l');
      } else ans.push_back('r');
    }
    reverse(ans.begin(), ans.end());
    cout << ans << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
