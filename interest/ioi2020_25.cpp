#include <bits/stdc++.h>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;

const int P = 998244353, R = 3;
const int BRUTE_N2_LIMIT = 50;

int mpow(int x, int k, int p = P) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll)x % p;
    x = x * (ll)x % p;
    k >>= 1;
  }
  return ret;
}

int norm(int x) { return x >= P ? x - P : x; }

struct NumberTheory {

  typedef pair<int, int> _P2_Field;

  mt19937 rng;

  NumberTheory() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}

  void _exGcd(int a, int b, int& x, int& y) {
    if (!b) {
      x = 1;
      y = 0;
      return;
    }
    _exGcd(b, a % b, y, x);
    y -= a / b * x;
  }

  int inv(int a, int p = P) {
    int x, y;
    _exGcd(a, p, x, y);
    if (x < 0)
      x += p;
    return x;
  }

  template <class Integer>
  bool quadRes(Integer a, Integer b) {
    if (a <= 1)
      return true;
    while (a % 4 == 0)
      a /= 4;
    if (a % 2 == 0)
      return (b % 8 == 1 || b % 8 == 7) == quadRes(a / 2, b);
    return ((a - 1) % 4 == 0 || (b - 1) % 4 == 0) == quadRes(b % a, a);
  }

  // assume p in prime, x in quadratic residue
  int sqrt(int x, int p = P) {
    if (p == 2 || x <= 1)
      return x;
    int w, v, k = (p + 1) / 2;
    do {
      w = rng() % p;
    } while (quadRes(v = int((w * (ll)w - x + p) % p), p));
    _P2_Field res(1, 0), a(w, 1);
    while (k) {
      if (k & 1)
        res = _P2_Field((res.first * (ll)a.first + res.second * (ll)a.second % p * v) % p, (res.first * (ll)a.second + res.second * (ll)a.first) % p);
      if (k >>= 1)
        a = _P2_Field((a.first * (ll)a.first + a.second * (ll)a.second % p * v) % p, (a.first * (ll)a.second << 1) % p);
    }
    return min(res.first, p - res.first);
  }

} nt;

template <class T, class Comp>
struct AdditionChain {
  int k;
  vector<T> prepare;
  T t, unit;
  Comp comp;

  AdditionChain(const T& t, const Comp& comp, int k, const T& unit = 1) : comp(comp), t(t), unit(unit), k(k), prepare(1U << k) {
    prepare[0] = unit;
    for (int i = 1; i < 1 << k; ++i)
      prepare[i] = comp(prepare[i - 1], t);
  }

  static AdditionChain fourRussians(const T &t, const Comp &comp, int lgn, const T &unit = 1) {
    lgn = max(lgn, 1);
    int k = 1, lglgn = 1;
    while (2 << lglgn <= lgn)
      ++lglgn;
    int w = lglgn / lgn;
    while (1 << k < w)
      ++k;
    return AdditionChain(t, comp, k, unit);
  }

  T pow(int n) const {
    if (n < 1 << k)
      return prepare[n];
    int r = n & ((1 << k) - 1);
    T step = pow(n >> k);
    for (int rep = 0; rep < k; ++rep)
      step = comp(step, step);
    return comp(step, prepare[r]);
  }
};

struct Simple {
  int n;
  vector<int> fac, ifac, inv;

  void build(int n) {
    this->n = n;
    fac.resize(n + 1);
    ifac.resize(n + 1);
    inv.resize(n + 1);
    fac[0] = 1;
    for (int x = 1; x <= n; ++x)
      fac[x] = fac[x - 1] * (ll)x % P;
    inv[1] = 1;
    for (int x = 2; x <= n; ++x)
      inv[x] = -(P / x) * (ll)inv[P % x] % P + P;
    ifac[0] = 1;
    for (int x = 1; x <= n; ++x)
      ifac[x] = ifac[x - 1] * (ll)inv[x] % P;
  }

  Simple() {
    build(1);
  }

  void check(int k) {
    int nn = n;
    if (k > nn) {
      while (k > nn)
        nn <<= 1;
      build(nn);
    }
  }

  int gfac(int k) {
    check(k);
    return fac[k];
  }

  int gifac(int k) {
    check(k);
    return ifac[k];
  }

  int ginv(int k) {
    check(k);
    return inv[k];
  }

  int binom(int n, int m) {
    if (m < 0 || m > n)
      return 0;
    return gfac(n) * (ll)gifac(m) % P * gifac(n - m) % P;
  }
} simp;

const int L2 = 11;

struct NTT {
  int L;
  int brev[1 << L2];
  vector<int> root;

  NTT() : L(-1) {
    for (int i = 1; i < (1 << L2); ++i)
      brev[i] = brev[i >> 1] >> 1 | ((i & 1) << (L2 - 1));
  }

  void prepRoot(int l) {
    L = l;
    root.resize(1 << L);
    int n = 1 << L;
    int primitive = mpow(R, (P - 1) >> L);
    root[0] = 1;
    for (int i = 1; i < n; ++i) root[i] = root[i - 1] * (ll)primitive % P;
  }

  void fft(int* a, int lgn, int d = 1) {
    if (L < lgn) prepRoot(lgn);
    int n = 1 << lgn;
    for (int i = 0; i < n; ++i) {
      int rev = (brev[i >> L2] | (brev[i & ((1 << L2) - 1)] << L2)) >> ((L2 << 1) - lgn);
      if (i < rev)
        swap(a[i], a[rev]);
    }
    int rt = d == 1 ? R : nt.inv(R);
    for (int k = L - 1, t = 1; t < n; t <<= 1, --k) {
      for (int i = 0; i < n; i += t << 1) {
        int *p1 = a + i, *p2 = a + i + t;
        for (int j = 0; j < t; ++j) {
          int x = p2[j] * (ll)root[j << k] % P;
          p2[j] = norm(p1[j] + P - x);
          p1[j] = norm(p1[j] + x);
        }
      }
    }
    if (d == -1) {
      reverse(a + 1, a + n);
      int nv = mpow(n, P - 2);
      for (int i = 0; i < n; ++i) a[i] = a[i] * (ll)nv % P;
    }
  }
} ntt;

struct Poly {
  vector<int> a;

  Poly(int v = 0) : a(1) {
    if ((v %= P) < 0)
      v += P;
    a[0] = v;
  }

  Poly(const vector<int>& a) : a(a) {}

  Poly(initializer_list<int> init) : a(init) {}

  // Helps
  int operator[](int k) const { return k < a.size() ? a[k] : 0; }

  int& operator[](int k) {
    if (k >= a.size())
      a.resize(k + 1);
    return a[k];
  }

  int deg() const { return a.size() - 1; }

  void redeg(int d) { a.resize(d + 1); }

  Poly monic() const;
  Poly sunic() const;

  Poly slice(int d) const {
    if (d < a.size())
      return vector<int>(a.begin(), a.begin() + d + 1);
    vector<int> res(a);
    res.resize(d + 1);
    return res;
  }

  int* base() { return a.begin().base(); }
  const int* base() const { return a.begin().base(); }

  Poly println(FILE* fp) const {
    fprintf(fp, "%d", a[0]);
    for (int i = 1; i < a.size(); ++i)
      fprintf(fp, " %d", a[i]);
    fputc('\n', fp);
    return *this;
  }

  // Calculations
  Poly operator+(const Poly& rhs) const {
    vector<int> res(max(a.size(), rhs.a.size()));
    for (int i = 0; i < res.size(); ++i)
      if ((res[i] = operator[](i) + rhs[i]) >= P)
        res[i] -= P;
    return res;
  }

  Poly operator-() const {
    Poly ret(a);
    for (int i = 0; i < a.size(); ++i)
      if (ret[i])
        ret[i] = P - ret[i];
    return ret;
  }

  Poly operator-(const Poly& rhs) const { return operator+(-rhs); }

  Poly operator*(const Poly& rhs) const;

  Poly operator/(const Poly& rhs) const;

  Poly operator%(const Poly& rhs) const;
};

Poly zeroes(int deg) { return vector<int>(deg + 1); }

struct Transposition {

  vector<int> _mul(int l, vector<int> res, const Poly& b) {
    vector<int> tmp(1 << l);
    memcpy(tmp.begin().base(), b.a.begin().base(), sizeof(int) * (b.deg() + 1));
    reverse(tmp.begin() + 1, tmp.end());
    ntt.fft(tmp.begin().base(), l, 1);
    for (int i = 0; i < (1 << l); ++i)
      res[i] = res[i] * (ll)tmp[i] % P;
    ntt.fft(res.begin().base(), l, -1);
    return res;
  }

  Poly bfMul(const Poly& a, const Poly& b) {
    int n = a.deg(), m = b.deg();
    Poly ret = zeroes(n - m);
    for (int i = 0; i <= n - m; ++i)
      for (int j = 0; j <= m; ++j)
        ret[i] = (ret[i] + a[i + j] * (ll)b[j]) % P;
    return ret;
  }

  Poly mul(const Poly& a, const Poly& b) {
    if (a.deg() < b.deg()) return 0;
    if (a.deg() <= BRUTE_N2_LIMIT) return bfMul(a, b);
    int l = 0;
    while ((1 << l) <= a.deg()) ++l;
    vector<int> res(1 << l);
    memcpy(res.begin().base(), a.a.begin().base(), sizeof(int) * (a.deg() + 1));
    ntt.fft(res.begin().base(), l, 1);
    res = _mul(l, res, b);
    res.resize(a.deg() - b.deg() + 1);
    return res;
  }

  pair<Poly, Poly> mul2(const Poly& a, const Poly& b1, const Poly& b2) {
    if (a.deg() <= BRUTE_N2_LIMIT) return make_pair(bfMul(a, b1), bfMul(a, b2));
    int l = 0;
    while ((1 << l) <= a.deg()) ++l;
    vector<int> fa(1 << l);
    memcpy(fa.begin().base(), a.a.begin().base(), sizeof(int) * (a.deg() + 1));
    ntt.fft(fa.begin().base(), l, 1);
    vector<int> res1 = _mul(l, fa, b1), res2 = _mul(l, fa, b2);
    res1.resize(a.deg() - b1.deg() + 1);
    res2.resize(a.deg() - b2.deg() + 1);
    return make_pair(res1, res2);
  }
} tp;

Poly operator "" _z(unsigned long long a) { return {0, (int)a}; }

Poly operator+(int v, const Poly& rhs) { return Poly(v) + rhs; }

Poly Poly::operator*(const Poly& rhs) const {
  int n = deg(), m = rhs.deg();
  if (n <= 10 || m <= 10 || n + m <= BRUTE_N2_LIMIT) {
    Poly ret = zeroes(n + m);
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= m; ++j)
        ret[i + j] = (ret[i + j] + a[i] * (ll)rhs[j]) % P;
    return ret;
  }
  n += m;
  int l = 0;
  while ((1 << l) <= n)
    ++l;
  vector<int> res(1 << l), tmp(1 << l);
  memcpy(res.begin().base(), base(), a.size() * sizeof(int));
  ntt.fft(res.begin().base(), l, 1);
  memcpy(tmp.begin().base(), rhs.base(), rhs.a.size() * sizeof(int));
  ntt.fft(tmp.begin().base(), l, 1);
  for (int i = 0; i < (1 << l); ++i)
    res[i] = res[i] * (ll)tmp[i] % P;
  ntt.fft(res.begin().base(), l, -1);
  res.resize(n + 1);
  return res;
}

const int N = 100010;

int n;
vector<int> g[N];
int p[N], ht[N], pref[N];
bool vis[N];

void dfs(int u) {
  vis[u] = true;
  for (int v : g[u])
    if (!vis[v]) {
      dfs(v);
      if (ht[u] < ht[v] + 1) {
        ht[u] = ht[v] + 1;
        pref[u] = v;
      }
    }
}

int dfn[N], id[N], t;
void hld(int u) {
  dfn[u] = ++t;
  id[t] = u;
  if (pref[u]) hld(pref[u]);
  for (int v : g[u])
    if (!dfn[v]) hld(v);
}

int pol;
int ls[N * 2], rs[N * 2], pred[N * 2], tak[N * 2];
Poly prod[N * 2], down[N * 2];

int label(int l, int r) {
  if (l == r) {
    pred[l] = 1;
    int u = id[l];
    for (int v : g[u])
      if (dfn[v] > dfn[u] && v != pref[u])
        pred[l] = max(pred[l], ht[v] + 2);
    return l;
  }
  int mid = (l + r) >> 1, o = ++pol;
  ls[o] = label(l, mid);
  rs[o] = label(mid + 1, r);
  pred[o] = max(pred[ls[o]], pred[rs[o]] + (mid - l + 1));
  prod[o] = prod[ls[o]] * prod[rs[o]];
  return o;
}

void solve(int o, int l, int r) {
  down[o].redeg(pred[o]);
  if (l == r) {
    int u = id[l];
    down[o] = tp.mul(down[o], prod[o]);
    for (int v : g[u])
      if (dfn[v] > dfn[u] && v != pref[u]) {
        down[tak[dfn[v]]] = down[o];
      }
    return;
  }
  int mid = (l + r) >> 1;
  down[ls[o]] = down[o];
  down[rs[o]] = tp.mul(down[o], prod[ls[o]]);
  solve(ls[o], l, mid);
  solve(rs[o], mid + 1, r);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  for (int i = 1; i <= n; ++i) {
    int a, b;
    cin >> a >> b;
    p[i] = a * (ll)nt.inv(b) % P;
  }
  dfs(1);
  hld(1);
  pol = n;
  for (int i = 1; i <= n; ++i) {
    prod[dfn[i]] = {norm(P + 1 - p[i]), p[i]};
  }
  for (int i = 1; i <= n;) {
    int d = ht[id[i]];
    tak[i] = label(i, i + d);
    i += d + 1;
  }
  Poly pr = zeroes(ht[1] + 1);
  fill(pr.a.begin(), pr.a.end(), 1);
  pr[0] = pr[1] = 0;
  for (int i = 2; i <= pr.deg(); ++i)
    if (pr[i])
      for (int j = i + i; j <= pr.deg(); j += i) pr[j] = 0;
  down[tak[1]] = pr;
  for (int i = 1; i <= n;) {
    int d = ht[id[i]];
    solve(tak[i], i, i + d);
    i += d + 1;
  }
  for (int i = 1; i <= n; ++i) cout << down[dfn[i]][0] << '\n';

  return 0;
}

