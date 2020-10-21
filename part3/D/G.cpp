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

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> m >> n;
  vector<vector<int>> a(n + 4, vector<int>(m + 4));
  for (int i = 0; i < n; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < m; ++j) {
      a[i + 2][j + 2] = s[j] == '#';
    }
  }
  while (true) {
    vector<vector<int>> b(n + 4, vector<int>(m + 4));
    auto tmp = a;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        b[i + 3][j + 3] = a[i + 2][j + 2];
        for (int u = 0; u < 3; ++u) {
          for (int v = 0; v < 3; ++v) {
            a[i + 2 + u][j + 2 + v] ^= b[i + 3][j + 3];
          }
        }
      }
    }
    int px = numeric_limits<int>::max(), py = numeric_limits<int>::max();
    for (int i = 0; i < n + 4; ++i) {
      for (int j = 0; j < m + 4; ++j) {
        if (a[i][j]) {
          px = min(px, i);
          py = min(py, j);
        }
      }
    }
    if (px < n + 2 && py < m + 2) {
      a[px][py] ^= 1;
      for (int i = px - 2; i < n; ++i) {
        for (int j = py - 2; j < m; ++j) {
          int t = a[i + 2][j + 2];
          b[i + 3][j + 3] ^= t;
          for (int u = 0; u < 3; ++u) {
            for (int v = 0; v < 3; ++v) {
              a[i + 2 + u][j + 2 + v] ^= t;
            }
          }
        }
      }
    }

    if (count(a.begin(), a.end(), vector<int>(m + 4, 0)) == n + 4 &&
        count(b.begin(), b.end(), vector<int>(m + 4, 0)) != n + 4) {
      swap(a, b);
    } else {
      a = tmp;
      break;
    }
  }
  int xl = numeric_limits<int>::max(), xr = 0, yl = xl, yr = 0;
  for (int i = 0; i < n + 4; ++i) {
    for (int j = 0; j < m + 4; ++j) {
      if (a[i][j]) {
        xl = min(xl, i);
        xr = max(xr, i);
        yl = min(yl, j);
        yr = max(yr, j);
      }
    }
  }
  for (int i = xl; i <= xr; ++i) {
    for (int j = yl; j <= yr; ++j) {
      cout << ".#"[a[i][j]];
    }
    cout << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
