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

const int N = 5010;

int Mod;

int mpow(int x, int k) {
  if (k == 0) return 1;
  int ret = mpow(R(x * (ull) x), k >> 1);
  if (k & 1) ret = R(ret * (ull) x);
  return R(ret);
}

void add(int &x, int y) { if ((x += y - Mod) < 0) x += Mod; }
int norm(int x) { return x >= Mod ? x - Mod : x; }

int n, k;
vector<int> g[N];
int x[N][N], y[N][N], a[N], b[N], l[N];

void dfs(int u, int dep) {
  if (g[u].empty()) {
    l[u] = 1;
    fill_n(x[u] + 1, n, 1);
    fill_n(y[u] + 1, n, 1);
    return;
  }
  int s = g[u][0], t = g[u][1];
  dfs(s, !dep);
  dfs(t, !dep);
  a[u] = a[s] + a[t] + (dep == 0);
  b[u] = b[s] + b[t] + (dep == 1);
  l[u] = l[s] + l[t];
  if (!dep) {
    int S = R(mpow(2, a[s]) * (ull) mpow(k, l[s])), T = R(mpow(2, a[t]) * (ull) mpow(k, l[t]));

    for (int i = 1; i <= n; ++i) x[u][i] = R(x[s][i] * (ull) x[t][i] * i);
    for (int i = 1; i <= n; ++i) y[u][i] = R(y[s][i] * (ull) T + y[t][i] * (ull) S);
  } else {
    int S = R(mpow(2, b[s]) * (ull) mpow(k, l[s])), T = R(mpow(2, b[t]) * (ull) mpow(k, l[t]));

    for (int i = 1; i <= n; ++i) y[u][i] = R(y[s][i] * (ull) y[t][i] * i);
    for (int i = 1; i <= n; ++i) x[u][i] = R(x[s][i] * (ull) T + x[t][i] * (ull) S);
  }
}

namespace ArbitraryModLagrange {

int coeff[N];

void _init(int n, int k) {
  vector<pair<int, int>> factorize;
  vector<int> mpk;
  int t = Mod;
  for (int i = 2; i * i <= t; ++i)
    if (t % i == 0) {
      int cnt = 0;
      while (t % i == 0) {
        t /= i;
        ++cnt;
      }
      factorize.emplace_back(i, cnt);
    }
  if (t != 1)
    factorize.emplace_back(t, 1);
  vector<int> pk(factorize.size());
  int prod = 1;

  auto mul = [&](int x, int sgn) {
    for (int i = 0; i < factorize.size(); ++i) {
      int p = factorize[i].first;
      while (x % p == 0) {
        x /= p;
        pk[i] += sgn;
      }
    }
    if (sgn == 1) prod = R(prod * (ull) x);
    else prod = R(prod * (ull) inv(x, Mod));
  };
  auto eval = [&]() {
    int ret = prod;
    for (int i = 0; i < factorize.size(); ++i)
      ret = R(ret * (ull) mpow(factorize[i].first, pk[i]));
    return ret;
  };

  for (int i = 1; i <= n; ++i) {
    mul(i, -1);
    mul(k - i, 1);
  }
  for (int i = 0; i <= n; ++i) {
    coeff[i] = eval();
    if (i & 1) coeff[i] = norm(Mod - coeff[i]);
    if (i == n) break;
    mul(n - i, 1);
    mul(k - i - 1, -1);
    mul(i + 1, -1);
    mul(k - i, 1);
  }
}

void init(int n, int k) {
  if (k <= n) coeff[k] = 1;
  else _init(n, k);
  for (int i = n; i; --i) add(coeff[i - 1], coeff[i]);
}

int calc(int *f) {
  ull ret = 0;
  for (int i = 0; i <= n; ++i) ret += f[i] * (ull) coeff[i];
  return R(ret);
}

}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k >> Mod;
  R = FastMod(Mod);
  for (int i = 2; i <= n; ++i) {
    int f;
    cin >> f;
    g[f].push_back(i);
  }
  dfs(1, 0);
  ArbitraryModLagrange::init(n, k);
  for (int u = 1; u <= n; ++u) {
    int ans = R(ArbitraryModLagrange::calc(x[u]) * (ull) ArbitraryModLagrange::calc(y[u]));
    cout << ans << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
