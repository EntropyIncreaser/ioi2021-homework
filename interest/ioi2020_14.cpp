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
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
 
#include <algorithm>
#include <tuple>
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

#define fir first
#define sec second

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
}

ostream &operator<<(ostream &os, const pair<char, int> &unit) {
  return os << unit.first << "^" << unit.second;
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

const int P = 998244353;

const int K = 10, N = 5000010;

int norm(int x) { return x >= P ? x - P : x; }

void add(int& x, int y) { if ((x += y - P) < 0) x += P; }

void sub(int& x, int y) { if ((x -= y) < 0) x += P; }

int mpow(int x, int k) {
  if (k == 0) return 1;
  int ret = mpow(x * (ull)x % P, k >> 1);
  if (k & 1) ret = ret * (ull)x % P;
  return ret;
}

int k, n;
int fac[N], ifac[N];
int a[K], p[K], q[K];
int v[N], e[N];

int binom(int n, int m) { return fac[n] * (ull)ifac[m] % P * ifac[n - m] % P; }

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> k;
  for (int i = 1; i <= k; ++i) cin >> a[i] >> p[i];
  int nv = mpow(100, P - 2);
  for (int i = 1; i <= k; ++i) {
    p[i] = p[i] * (ull)nv % P;
    q[i] = P + 1 - p[i];
  }

  n = accumulate(a + 1, a + k + 1, 0);
  fac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * (ull)i % P;
  ifac[n] = mpow(fac[n], P - 2);
  for (int i = n; i; --i) ifac[i - 1] = ifac[i] * (ull)i % P;
  for (int i = a[1] - 1; i <= n; ++i) v[i] = binom(i, a[1] - 1);
  int prod = mpow(p[1], a[1]);
  for (int i = a[1] - 1; i <= n; ++i) {
    v[i] = v[i] * (ull)prod % P;
    prod = prod * (ull)q[1] % P;
  }
  for (int i = 2; i <= k; ++i) {
    int w = mpow(p[i], a[i]);
    prod = 1;
    for (int j = a[i]; j <= n; ++j) {
      prod = (prod + (P - w) * (ull)binom(j - 1, a[i] - 1)) % P;
      w = w * (ull)q[i] % P;
      v[j] = v[j] * (ull)prod % P;
    }
  }
  prod = 1;
  for (int i = 1; i <= k; ++i) prod = prod * (ull)q[i] % P;
  prod = mpow(prod, P - 2);
  int w = 1;
  for (int i = 0; i <= n; ++i) {
    v[i] = v[i] * (ull)w % P;
    w = w * (ull)prod % P;
  }
  prod = mpow(prod, P - 2);
  int t = prod * (ull)mpow(P + 1 - prod, P - 2) % P;
  int mu = mpow(t, n);
  for (int i = 0; i <= n; ++i) {
    e[i] = mu * (ull)binom(n, i) % P;
    if ((n ^ i) & 1) e[i] = norm(P - e[i]);
  }
  sub(e[0], 1);
  mu = mpow(P - t - 1, P - 2);
  for (int i = 0; i <= n; ++i) e[i] = e[i] * (ull)mu % P;
  mu = (P - t) * (ull)mu % P;
  for (int i = 1; i <= n; ++i) e[i] = (e[i] + e[i - 1] * (ull)mu) % P;
  int ans = 0;
  for (int i = 0; i < n; ++i) ans = (ans + e[i] * (ull)v[i]) % P;
  ans = ans * (ull)mpow(P + 1 - prod, P - 2) % P;
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
