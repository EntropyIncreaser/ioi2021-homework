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
#include <iomanip>
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

typedef __uint128_t L;

struct FastMod {
  ull b, m;

  FastMod(ull b) : b(b), m(ull((L(1) << 64) / b)) {}

  ull reduce(ull a) {
    ull q = (ull) ((L(m) * a) >> 64);
    ull r = a - q * b;  // can be proven that 0 <= r < 2*b
    return r >= b ? r - b : r;
  }
};

const int N = 4010;

int Mod;
int fib[N], prod[N];

int norm(int x) { return x >= Mod ? x - Mod : x; }

void add(int &x, int y) { if ((x += y - Mod) < 0) x += Mod; }

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    int k;
    cin >> k >> Mod;
    FastMod barret(Mod);
    fib[0] = fib[1] = 1;
    for (int i = 2; i <= k; ++i) fib[i] = norm(fib[i - 1] + fib[i - 2]);
    fill_n(prod, k + 1, 0);
    prod[0] = 1;
    for (int i = 1; i <= k; ++i)
      for (int j = i; j; --j)
        prod[j] = barret.reduce(prod[j] + prod[j - 1] * (ull) (Mod - fib[i]));
    int ans = 0;
    for (int i = k; i; --i) {
      int x;
      cin >> x;
      ans = barret.reduce(ans + x * (ull) prod[i]);
    }
    ans = norm(Mod - ans);
    cout << ans << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
