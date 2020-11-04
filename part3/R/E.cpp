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

const int Base0 = 11451419, Base1 = 19198101, Mod = 998244353;

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ull) x % Mod;
    x = x * (ull) x % Mod;
    k >>= 1;
  }
  return ret;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("easy.in", "r", stdin);
  freopen("easy.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int l;
  string s;
  cin >> l;
  cin.ignore();
  getline(cin, s);
  int n, m;
  cin >> n >> m;
  vector<string> v(n);
  cin >> v;
  int mx = n, my = m;
  vector<int> pw(n), qw(m);
  pw[0] = 1;
  for (int i = 1; i < n; ++i) pw[i] = pw[i - 1] * (ull) Base0 % Mod;
  qw[0] = 1;
  for (int i = 1; i < m; ++i) qw[i] = qw[i - 1] * (ull) Base1 % Mod;
  int pattern = 0, cnt = 0;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      if (v[i][j] == 'X') {
        ++cnt;
        mx = min(mx, i);
        my = min(my, j);
        pattern = (pattern + pw[i] * (ull) qw[j]) % Mod;
      }
  pattern = pattern * (ull) mpow(Base0, Mod - 1 - mx) % Mod * mpow(Base1, Mod - 1 - my) % Mod;
  pw.resize(l * 2 + 1);
  pw[0] = mpow(Base0, Mod - 1 - l);
  for (int i = 1; i != pw.size(); ++i) pw[i] = pw[i - 1] * (ull) Base0 % Mod;
  qw.resize(l * 2 + 1);
  qw[0] = mpow(Base1, Mod - 1 - l);
  for (int i = 1; i != qw.size(); ++i) qw[i] = qw[i - 1] * (ull) Base1 % Mod;

  vector<int> px(l * 2 + 1), py(l * 2 + 1);
  mx = my = l * 2;
  auto ins = [&](vector<int> &p, int &m, int v) {
    v += l;
    ++p[v];
    m = min(m, v);
  };
  auto ers = [&](vector<int> &p, int &m, int v) {
    v += l;
    --p[v];
    if (!p[m]) ++m;
  };

  map<pair<int, int>, int> mp;
  vector<int> posx(l + 1), posy(l + 1);
  for (int i = 0; i < l; ++i) {
    posx[i + 1] = posx[i];
    posy[i + 1] = posy[i];
    switch (s[i]) {
      case 'u':
        --posx[i + 1];
        break;
      case 'd':
        ++posx[i + 1];
        break;
      case 'l':
        --posy[i + 1];
        break;
      case 'r':
        ++posy[i + 1];
        break;
      default:
        break;
    }
  }
  int b = 0, hash = 0;
  bool fl = false;
  for (int e = 0; e <= l; ++e) {
    int x = posx[e], y = posy[e];
    if (++mp[make_pair(x, y)] == 1) {
      ins(px, mx, x);
      ins(py, my, y);
      hash = (hash + pw[x + l] * (ull) qw[y + l]) % Mod;
    }
    while (mp.size() > cnt) {
      x = posx[b];
      y = posy[b];
      ++b;
      if (--mp[make_pair(x, y)] == 0) {
        mp.erase(make_pair(x, y));
        ers(px, mx, x);
        ers(py, my, y);
        hash = (hash + (Mod - pw[x + l]) * (ull) qw[y + l]) % Mod;
      }
    }
    int cur = hash * (ull) pw[2 * l - mx] % Mod * qw[2 * l - my] % Mod;
    if (cur == pattern) {
      fl = true;
      cout << "YES\n" << b + 1 << ' ' << e << '\n';
      break;
    }
  }
  if (!fl) cout << "NO\n";

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
