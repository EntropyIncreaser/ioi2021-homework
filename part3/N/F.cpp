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

const int P = 1000003;

int norm(int x) { return x >= P ? (x - P) : x; }

void add(int &x, int y) { if ((x += y) >= P) x -= P; }

void sub(int &x, int y) { if ((x -= y) < 0) x += P; }

void exGcd(int a, int b, int &x, int &y) {
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

const int N = 200010;

int l[N], t[N];
int fac[N * 2], ifac[N];
int aw[N], bw[N];

int C(int n, int m) { return fac[n] * (ull) ifac[m] % P * ifac[n - m] % P; }

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, a, b, c;
  cin >> n >> a >> b >> c;
  swap(a, b);
  for (int i = 1; i <= n; ++i)cin >> l[i];
  for (int i = 1; i <= n; ++i)cin >> t[i];
  fac[0] = 1;
  for (int i = 1; i <= n * 2; ++i)fac[i] = fac[i - 1] * (ull) i % P;
  ifac[n] = inv(fac[n]);
  for (int i = n; i; --i)ifac[i - 1] = ifac[i] * (ull) i % P;
  aw[0] = 1;
  for (int i = 1; i <= n; ++i)aw[i] = aw[i - 1] * (ull) a % P;
  bw[0] = 1;
  for (int i = 1; i <= n; ++i)bw[i] = bw[i - 1] * (ull) b % P;
  int ans = 0;
  for (int i = 2; i <= n; ++i)
    ans = (ans + l[i] * (ull) aw[n - i] % P * bw[n - 1] % P * C(n * 2 - i - 2, n - 2)) % P;
  for (int i = 2; i <= n; ++i)
    ans = (ans + t[i] * (ull) bw[n - i] % P * aw[n - 1] % P * C(n * 2 - i - 2, n - 2)) % P;
  int sum = 0, tot = 1;
  n -= 2;
  for (int i = 0; i <= n; ++i) {
    add(sum, tot);
    tot = tot * (ull) (a + b) % P;
  }
  for (int i = n + 1; i <= n * 2; ++i) {
    tot = (tot + (aw[n + 1] * (ull) bw[i - n - 1] + bw[n + 1] * (ull) aw[i - n - 1]) % P * (P - C(i - 1, n))) % P;
    add(sum, tot);
    tot = tot * (ull) (a + b) % P;
  }
  ans = (ans + sum * (ull) c) % P;
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
