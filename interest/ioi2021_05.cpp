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
#include <bits/stdc++.h>
#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef unsigned long long ull;

typedef __uint128_t L;

struct FastMod {
  ull b, m;

  FastMod(ull b) : b(b), m(ull((L(1) << 64) / b)) {}

  ull operator()(ull a) {
    ull q = (ull) ((L(m) * a) >> 64);
    ull r = a - q * b;  // can be proven that 0 <= r < 2*b
    return r >= b ? r - b : r;
  }
} R(2);

int Mod;

void add(int &x, int y) { if ((x += y - Mod) < 0) x += Mod; }

void sub(int &x, int y) { if ((x -= y) < 0) x += Mod; }

int norm(int x) { return x >= Mod ? x - Mod : x; }

const int N = 1e7 + 10;

int fac[N], der[N], I[N];

int main() {
  int n;
  cin >> n >> Mod;
  R = FastMod(Mod);
  fac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = R(fac[i - 1] * (ull) i);
  der[0] = 1;
  for (int i = 1; i <= n; ++i) {
    der[i] = R(der[i - 1] * (ull) i);
    if (i & 1) sub(der[i], 1);
    else add(der[i], 1);
  }
  I[0] = I[1] = 1;
  int pw = 1;
  for (int i = 2; i <= n; ++i)
    if (i & 1) {
      I[i] = I[i - 1];
      add(I[i], norm(I[i - 2] << 1));
      sub(I[i], norm(I[i - 3] << 1));
      I[i] = R(I[i] * (ull) (i - 1) + I[i - 1]);
    } else {
      pw = norm(pw << 1);
      I[i] = R(I[i - 1] * (ull) i + I[i - 2] * (ull) (Mod - i * 2) + pw);
    }
  int ans = 0;
  for (int i = 1; i <= n; ++i) {
    int f = fac[i];
    sub(f, norm(der[i] << 1));
    if (i & 1) {
      add(f, I[i]);
      sub(f, I[i - 1]);
    } else add(f, I[i]);
    ans ^= f;
  }
  cout << ans << '\n';
  return 0;
}
