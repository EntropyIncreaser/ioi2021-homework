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
#include <unordered_map>
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

ull mulInv(ull x) {
  ull y = 1;
  while (x * y != 1) {
    y = y * (2 - x * y);
  }
  return y;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<ull> b(n);
  vector<ull> a(n);
  cin >> b;
  ull s;
  cin >> s;
  ull ans = 0;
  if (n <= 42) {
    map<ull, ull> giant;
    int x = n / 2;
    function<void(int, ull, ull)> dfs = [&](int k, ull u, ull v) {
      if (k == x) {
        giant.insert(make_pair(u, v));
        return;
      }
      dfs(k + 1, u, v);
      dfs(k + 1, u + b[k], v | 1ull << k);
    };
    dfs(0, 0, 0);
    dfs = [&](int k, ull u, ull v) {
      if (k == n) {
        auto it = giant.find(s - u);
        if (it != giant.end()) {
          ans = v | it->second;
        }
        return;
      }
      dfs(k + 1, u, v);
      dfs(k + 1, u + b[k], v | 1ull << k);
    };
    dfs(x, 0, 0);
  } else {
    ull lb = b[0] & -b[0], bt = b[0] / lb, lim = (ull(-1) / lb) >> (n - 1);
    int ctz = __builtin_ctzll(lb);
    ull inv = mulInv(bt);
    for (ull a0 = 1; a0 <= lim; a0 += 2) {
      ull r = a0 * inv;
      for (ull lc = 0; lc != lb; ++lc) {
        ull r1 = r + (lc ? (lc << (64 - ctz)) : 0);
        for (int i = 0; i < n; ++i) {
          a[i] = b[i] * r1;
        }
        ull u = s * r1, t = 0;
        for (int i = n - 1; i >= 0; --i) {
          if (u >= a[i]) {
            u -= a[i];
            t |= 1ULL << i;
          }
        }
        if (u == 0) {
          ans = t;
        }
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    cout << ((ans >> i) & 1);
  }
  cout << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
