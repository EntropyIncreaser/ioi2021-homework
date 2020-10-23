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

const int P = 998244353;

int norm(int x) { return x >= P ? (x - P) : x; }

void add(int& x, int y) { if ((x += y) >= P) x -= P; }

void sub(int& x, int y) { if ((x -= y) < 0) x += P; }

void exGcd(int a, int b, int& x, int& y) {
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exGcd(b, a % b, y, x);
  y -= a / b * x;
}

int inv(int a) {
  int x, y;
  exGcd(a, P, x, y);
  return norm(x + P);
}

#ifdef ELEGIA
const int N = 100;
#else
const int N = 15000010;
#endif

int fac[N * 2], ifac[N], nv[N];

int C(int n, int m) {
  return (m < 0 || m > n) ? 0 : int(fac[n] * (ull)ifac[m] % P * ifac[n - m] % P);
}

int sC(int n, int m) {
  return (m & 1) ? norm(P - C(n, m)) : C(n, m);
}

int nC(int n, int m) {
  return C(n + m - 1, m);
}

void calc(int f[], int n, int r) {
  f[0] = 1;
  int c = (P - n) * (ull)C(n - 1, r) % P;
  for (int i = 0; i <= n - r - 1; ++i) {
    f[i + r + 1] = sC(n - r - 1, i) * (ull)c % P * nv[i + r + 1] % P;
  }
}

int f[N], g[N], h[N];
bool vis[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  fac[0] = 1;
  for (int i = 1; i < N * 2; ++i) fac[i] = fac[i - 1] * (ull)i % P;
  ifac[N - 1] = inv(fac[N - 1]);
  for (int i = N - 1; i; --i) ifac[i - 1] = ifac[i] * (ull)i % P;
  for (int i = N - 1; i; --i) nv[i] = fac[i - 1] * (ull)ifac[i] % P;

  int n, d, r;
  cin >> n >> d >> r;
  calc(f, n, r);
  calc(g, n - 1, r - 1);
  for (int i = 1; i <= d; ++i) {
    h[i] = ((P - r) * (ull)f[i] + g[i - 1] * (ull)n) % P;
  }
  for (int x = 2; x <= d; ++x) {
    if (!vis[x]) {
      for (int i = 1, y = x; y <= d; ++i, y += x) {
        add(h[y], h[i]);
        vis[y] = true;
      }
    }
  }
  int ans = 0;
  for (int i = 0; i <= d; ++i) {
    ans = (ans + h[d - i] * (ull)nC(n, i)) % P;
  }
  ans = ans * (ull)inv(nC(n, d)) % P;
  add(ans, r);
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
