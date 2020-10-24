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

const int N = 200010;

int n;
int x[N], y[N], u[N];
int t[4][4];
int cnt[4][2];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("integral.in", "r", stdin);
  freopen("integral.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> x[i] >> y[i];
    x[i] &= 1;
    y[i] &= 1;
  }
  x[0] = x[n];
  y[0] = y[n];
  int tot = 0;
  for (int i = 1; i <= n; ++i) {
    tot ^= (x[i - 1] & y[i]) ^ (x[i] & y[i - 1]);
  }
  if (tot) {
    cout << "0\n";
    return 0;
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      t[i][j] = (i & (j >> 1) & 1) ^ (j & (i >> 1) & 1);
    }
  }
  for (int i = 0; i <= n; ++i) {
    u[i] = (x[i] << 1) | y[i];
  }
  ll ans = 0;
  for (int i = 1; i <= n; ++i) {
    static int tmp[4][2];
    memset(tmp, 0, sizeof(tmp));
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k <= 1; ++k) {
        tmp[j][k ^ t[u[i - 1]][u[i]]] += cnt[j][k];
      }
    }
    memcpy(cnt, tmp, sizeof(cnt));
    for (int j = 0; j < 4; ++j) {
      ans += cnt[j][t[j][u[i]]];
    }
    ++cnt[u[i]][0];
  }
  cout << ans - n << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
