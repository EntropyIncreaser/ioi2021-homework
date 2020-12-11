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

void exGcd(int a, int b, int &x, int &y) {
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exGcd(b, a % b, y, x);
  y -= a / b * x;
}

int inv(int a, int mod) {
  int x, y;
  exGcd(a, mod, x, y);
  if (x < 0)
    x += mod;
  return x;
}

const int N = 1010;

int n, Mod;
int fac[N], ifac[N];

int gf[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> Mod;
  R = FastMod(Mod);
  fac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = R((ull) fac[i - 1] * i);
  ifac[n] = inv(fac[n], Mod);
  for (int i = n; i; --i) ifac[i - 1] = R((ull) ifac[i] * i);
  for (int k = n - 1; k >= 0; --k) {
    gf[0] = 1;
    for (int i = 1; i <= n; ++i) {
      ull v = 0;
      int j, t = 0;
      for (j = 1; j + k <= i; j += k + 1) {
        v += (ull) gf[i - j] * ifac[j] + (ull) (Mod - gf[i - j - k]) * ifac[j + k];
        if ((++t & 7) == 7) v = R(v);
      }
      v += (ull) gf[i - j] * ifac[j];
      gf[i] = R(v);
    }
    cout << R((ull) (Mod + 1 - gf[n]) * fac[n]) << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

