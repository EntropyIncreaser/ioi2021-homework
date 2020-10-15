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

#define popcnt __builtin_popcount

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

const int N = 20;

int n, m, sum;
int masku[N], maskv[N];
int u[N], v[N];
bool s[1 << N], t[1 << N];
int n1, m1;
int x[1 << N], y[1 << N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 0; i < n; ++i) {
    string str;
    cin >> str;
    for (int j = 0; j < m; ++j) {
      if (str[j] == '1') {
        masku[i] |= 1 << j;
        maskv[j] |= 1 << i;
      }
    }
  }
  for (int i = 0; i != 1 << n; ++i) {
    int mask = 0;
    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) {
        mask |= masku[j];
      }
    }
    s[i] = popcnt(i) <= popcnt(mask);
  }
  for (int i = 0; i != 1 << m; ++i) {
    int mask = 0;
    for (int j = 0; j < m; ++j) {
      if ((i >> j) & 1) {
        mask |= maskv[j];
      }
    }
    t[i] = popcnt(i) <= popcnt(mask);
  }
  for (int i = 0; i != 1 << n; ++i) {
    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) {
        s[i] &= s[i ^ 1 << j];
      }
    }
  }
  for (int i = 0; i != 1 << m; ++i) {
    for (int j = 0; j < m; ++j) {
      if ((i >> j) & 1) {
        t[i] &= t[i ^ 1 << j];
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    cin >> u[i];
  }
  for (int i = 0; i < m; ++i) {
    cin >> v[i];
  }
  cin >> sum;
  for (int i = 0; i != 1 << n; ++i) {
    if (!s[i]) {
      continue;
    }
    int msum = 0;
    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) {
        msum += u[j];
      }
    }
    x[n1++] = msum;
  }
  for (int i = 0; i != 1 << m; ++i) {
    if (!t[i]) {
      continue;
    }
    int msum = 0;
    for (int j = 0; j < m; ++j) {
      if ((i >> j) & 1) {
        msum += v[j];
      }
    }
    y[m1++] = msum;
  }
  sort(x, x + n1);
  sort(y, y + m1);
  int p = n1;
  ll ans = 0;
  for (int i = 0; i < m1; ++i) {
    while (p && x[p - 1] + y[i] >= sum) {
      --p;
    }
    ans += n1 - p;
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
