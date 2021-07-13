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

#include <algorithm>
#include <chrono>
#include <random>
#include <functional>
#include <vector>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

const int P = 998244353, R = 3;
const int BRUTE_N2_LIMIT = 50;

int mpow(int x, int k, int p = P) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll) x % p;
    x = x * (ll) x % p;
    k >>= 1;
  }
  return ret;
}

int norm(int x) { return x >= P ? x - P : x; }

void add(int &x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int &x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int reduce(int x) { return x < 0 ? x + P : x; }

struct NumberTheory {

  typedef pair<int, int> _P2_Field;

  mt19937 rng;

  NumberTheory() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}

  void _exGcd(int a, int b, int &x, int &y) {
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

  template<class Integer>
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
    } while (quadRes(v = int((w * (ll) w - x + p) % p), p));
    _P2_Field res(1, 0), a(w, 1);
    while (k) {
      if (k & 1)
        res = _P2_Field((res.first * (ll) a.first + res.second * (ll) a.second % p * v) % p,
                        (res.first * (ll) a.second + res.second * (ll) a.first) % p);
      if (k >>= 1)
        a = _P2_Field((a.first * (ll) a.first + a.second * (ll) a.second % p * v) % p,
                      (a.first * (ll) a.second << 1) % p);
    }
    return min(res.first, p - res.first);
  }

} nt;

template<class T, class Comp>
struct AdditionChain {
  int k;
  vector<T> prepare;
  T t, unit;
  Comp comp;

  AdditionChain(const T &t, const Comp &comp, int k, const T &unit = 1) : comp(comp), t(t), unit(unit), k(k),
                                                                          prepare(1U << k) {
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
      fac[x] = fac[x - 1] * (ll) x % P;
    inv[1] = 1;
    for (int x = 2; x <= n; ++x)
      inv[x] = -(P / x) * (ll) inv[P % x] % P + P;
    ifac[0] = 1;
    for (int x = 1; x <= n; ++x)
      ifac[x] = ifac[x - 1] * (ll) inv[x] % P;
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
    return gfac(n) * (ll) gifac(m) % P * gifac(n - m) % P;
  }
} simp;

const int L2 = 11;
struct NTT {
  int L;
  vector<int> root;

  NTT() : L(-1) {}

  void prepRoot(int l) {
    L = l;
    root.resize((1 << L) + 1);
    int i, n = 1 << L;
    int *w2 = root.data();
    *w2 = 1;
    w2[1 << l] = mpow(31, 1 << (21 - l));

    for (i = l; i; --i)
      w2[1 << (i - 1)] = (ull) w2[1 << i] * w2[1 << i] % P;

    for (i = 1; i < n; ++i)
      w2[i] = (ull) w2[i & (i - 1)] * w2[i & -i] % P;
  }

  void DIF(int *a, int l) {
    int *j, *k, n = 1 << l, len = n >> 1, r, *o;

    for (; len; len >>= 1)
      for (j = a, o = root.data(); j != a + n; j += len << 1, ++o)
        for (k = j; k != j + len; ++k) {
          r = (ull) *o * k[len] % P;
          k[len] = reduce(*k - r);
          add(*k, r);
        }
  }

  void DIT(int *a, int l) {
    int *j, *k, n = 1 << l, len = 1, r, *o;

    for (; len != n; len <<= 1)
      for (j = a, o = root.data(); j != a + n; j += len << 1, ++o)
        for (k = j; k != j + len; ++k) {
          r = reduce(*k + k[len] - P);
          k[len] = ull(*k - k[len] + P) * *o % P;
          *k = r;
        }
  }

  void fft(int *a, int lgn, int d = 1) {
    if (L < lgn) prepRoot(lgn);
    int n = 1 << lgn;
    if (d == 1) DIF(a, lgn);
    else {
      DIT(a, lgn);
      reverse(a + 1, a + n);
      ull nv = P - (P - 1) / n;
      for (int i = 0; i < n; ++i) a[i] = a[i] * nv % P;
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

  Poly(const vector<int> &a) : a(a) {}

  Poly(initializer_list<int> init) : a(init) {}

  // Helps
  int operator[](int k) const { return k < a.size() ? a[k] : 0; }

  int &operator[](int k) {
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

  int *base() { return a.data(); }

  const int *base() const { return a.data(); }

  Poly println(FILE *fp) const {
    fprintf(fp, "%d", a[0]);
    for (int i = 1; i < a.size(); ++i)
      fprintf(fp, " %d", a[i]);
    fputc('\n', fp);
    return *this;
  }

  // Calculations
  Poly operator+(const Poly &rhs) const {
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

  Poly operator-(const Poly &rhs) const { return operator+(-rhs); }

  Poly operator*(const Poly &rhs) const;

  Poly operator/(const Poly &rhs) const;

  Poly operator%(const Poly &rhs) const;

  Poly der() const; // default: remove trailing
  Poly integ() const;

  Poly inv() const;

  Poly sqrt() const;

  Poly ln() const;

  Poly exp() const;

  pair<Poly, Poly> sqrti() const;

  pair<Poly, Poly> expi() const;

  Poly quo(const Poly &rhs) const;

  pair<Poly, Poly> iquo(const Poly &rhs) const;

  pair<Poly, Poly> div(const Poly &rhs) const;

  Poly taylor(int k) const;

  Poly pow(int k) const;

  Poly exp(int k) const;
};

Poly zeroes(int deg) { return vector<int>(deg + 1); }

struct Newton {
  void inv(const Poly &f, const Poly &nttf, Poly &g, const Poly &nttg, int t) {
    int n = 1 << t;
    Poly prod = nttf;
    for (int i = 0; i < (n << 1); ++i)
      prod[i] = prod[i] * (ll) nttg[i] % P;
    ntt.fft(prod.base(), t + 1, -1);
    for (int i = 0; i < n; ++i)
      prod[i] = 0;
    ntt.fft(prod.base(), t + 1, 1);
    for (int i = 0; i < (n << 1); ++i)
      prod[i] = prod[i] * (ll) nttg[i] % P;
    ntt.fft(prod.base(), t + 1, -1);
    for (int i = 0; i < n; ++i)
      prod[i] = 0;
    g = g - prod;
  }

  void inv(const Poly &f, const Poly &nttf, Poly &g, int t) {
    Poly nttg = g;
    nttg.redeg((2 << t) - 1);
    ntt.fft(nttg.base(), t + 1, 1);
    inv(f, nttf, g, nttg, t);
  }

  void inv(const Poly &f, Poly &g, int t) {
    Poly nttg = g;
    nttg.redeg((2 << t) - 1);
    ntt.fft(nttg.base(), t + 1, 1);
    Poly nttf = f;
    nttf.redeg((2 << t) - 1);
    ntt.fft(nttf.base(), t + 1, 1);
    inv(f, nttf, g, nttg, t);
  }

  void sqrt(const Poly &f, Poly &g, Poly &nttg, Poly &h, int t) {
    for (int i = 0; i < (1 << t); ++i)
      nttg[i] = mpow(nttg[i], 2);
    ntt.fft(nttg.base(), t, -1);
    nttg = nttg - f;
    for (int i = 0; i < (1 << t); ++i)
      if ((nttg[i + (1 << t)] += nttg[i]) >= P)
        nttg[i + (1 << t)] -= P;
    memset(nttg.base(), 0, sizeof(int) << t);
    ntt.fft(nttg.base(), t + 1, 1);
    Poly tmp = h;
    tmp.redeg((2 << t) - 1);
    ntt.fft(tmp.base(), t + 1, 1);
    for (int i = 0; i < (2 << t); ++i)
      tmp[i] = tmp[i] * (ll) nttg[i] % P;
    ntt.fft(tmp.base(), t + 1, -1);
    memset(tmp.base(), 0, sizeof(int) << t);
    g = g - tmp * nt.inv(2);
  }

  void exp(const Poly &f, Poly &g, Poly &nttg, Poly &h, int t) {
    Poly ntth(h);
    ntt.fft(ntth.base(), t, 1);
    Poly dg = g.der().slice((1 << t) - 1);
    ntt.fft(dg.base(), t, 1);
    Poly tmp = zeroes((1 << t) - 1);
    for (int i = 0; i < (1 << t); ++i) {
      tmp[i] = nttg[i << 1] * (ll) ntth[i] % P;
      dg[i] = dg[i] * (ll) ntth[i] % P;
    }
    ntt.fft(tmp.base(), t, -1);
    ntt.fft(dg.base(), t, -1);
    if (--tmp[0] < 0)
      tmp[0] = P - 1;
    dg.redeg((2 << t) - 1);
    Poly df0 = f.der().slice((1 << t) - 1);
    df0[(1 << t) - 1] = 0;
    for (int i = 0; i < (1 << t); ++i) {
      if ((dg[i | 1 << t] = dg[i] - df0[i]) < 0)
        dg[i | 1 << t] += P;
    }
    memcpy(dg.base(), df0.base(), sizeof(int) * ((1 << t) - 1));
    tmp.redeg((2 << t) - 1);
    ntt.fft(tmp.base(), t + 1, 1);
    df0.redeg((2 << t) - 1);
    ntt.fft(df0.base(), t + 1, 1);
    for (int i = 0; i < (2 << t); ++i)
      df0[i] = df0[i] * (ll) tmp[i] % P;
    ntt.fft(df0.base(), t + 1, -1);
    memcpy(df0.base() + (1 << t), df0.base(), sizeof(int) << t);
    memset(df0.base(), 0, sizeof(int) << t);
    dg = (dg - df0).integ().slice((2 << t) - 1) - f;
    ntt.fft(dg.base(), t + 1, 1);
    for (int i = 0; i < (2 << t); ++i)
      tmp[i] = dg[i] * (ll) nttg[i] % P;
    ntt.fft(tmp.base(), t + 1, -1);
    g.redeg((2 << t) - 1);
    for (int i = 1 << t; i < (2 << t); ++i)
      if (tmp[i])
        g[i] = P - tmp[i];
  }
} nit;

struct SemiRelaxedConvolution {

  template<class Function>
  void run(const vector<int> &a, vector<int> &b, const Function &relax) {
    int n = a.size() - 1;
    function<void(int, int)> divideConquer = [&](int l, int r) {
      if (r - l <= BRUTE_N2_LIMIT) {
        for (int i = l; i <= r; ++i) {
          for (int j = l; j < i; ++j)
            b[i] = (b[i] + b[j] * (ll) a[i - j]) % P;
          relax(i);
        }
        return;
      }
      int lg = 31 - __builtin_clz(r - l) + 1;
      int d = (r - l) / lg + 1;
      int lgd = 0;
      while ((1 << lgd) < d) ++lgd;
      ++lgd;

      vector<int> top((lg << (lgd + 1)));
      for (int i = 0; i < lg; ++i) {
        copy(a.begin() + i * d, a.begin() + min((i + 2) * d, n + 1), top.begin() + (i << lgd));
        ntt.fft(top.data() + (i << lgd), lgd, 1);
      }
      for (int i = 0; i < lg; ++i) {
        if (i)
          ntt.fft(top.data() + ((lg + i) << lgd), lgd, -1);
        for (int j = 0; j < min(d, r - l - i * d + 1); ++j)
          b[l + i * d + j] = norm(b[l + i * d + j] + top[((lg + i) << lgd) + d + j]);
        divideConquer(l + i * d, min(l + (i + 1) * d - 1, r));
        if (i + 1 < lg) {
          copy(b.begin() + l + i * d, b.begin() + min(l + (i + 1) * d, n + 1), top.begin() + ((lg + i) << lgd));
          fill(top.data() + ((lg + i) << lgd) + d, top.data() + ((lg + i + 1) << lgd), 0);
          ntt.fft(top.data() + ((lg + i) << lgd), lgd, 1);
        }
        for (int j = i + 1; j < lg; ++j) {
          for (int k = 0; k < (1 << lgd); ++k)
            top[((lg + j) << lgd) + k] =
                    (top[((lg + j) << lgd) + k] + top[((j - i - 1) << lgd) + k] * (ll) top[((lg + i) << lgd) + k]) % P;
        }
      }
    };
    divideConquer(0, n);
  }
} src;

struct Transposition {

  vector<int> _mul(int l, vector<int> res, const Poly &b) {
    vector<int> tmp(1 << l);
    memcpy(tmp.data(), b.a.data(), sizeof(int) * (b.deg() + 1));
    reverse(tmp.begin() + 1, tmp.end());
    ntt.fft(tmp.data(), l, 1);
    for (int i = 0; i < (1 << l); ++i)
      res[i] = res[i] * (ll) tmp[i] % P;
    ntt.fft(res.data(), l, -1);
    return res;
  }

  Poly bfMul(const Poly &a, const Poly &b) {
    int n = a.deg(), m = b.deg();
    Poly ret = zeroes(n - m);
    for (int i = 0; i <= n - m; ++i)
      for (int j = 0; j <= m; ++j)
        ret[i] = (ret[i] + a[i + j] * (ll) b[j]) % P;
    return ret;
  }

  Poly mul(const Poly &a, const Poly &b) {
    if (a.deg() < b.deg()) return 0;
    if (a.deg() <= BRUTE_N2_LIMIT) return bfMul(a, b);
    int l = 0;
    while ((1 << l) <= a.deg()) ++l;
    vector<int> res(1 << l);
    memcpy(res.data(), a.a.data(), sizeof(int) * (a.deg() + 1));
    ntt.fft(res.data(), l, 1);
    res = _mul(l, res, b);
    res.resize(a.deg() - b.deg() + 1);
    return res;
  }

  pair<Poly, Poly> mul2(const Poly &a, const Poly &b1, const Poly &b2) {
    if (a.deg() <= BRUTE_N2_LIMIT) return make_pair(bfMul(a, b1), bfMul(a, b2));
    int l = 0;
    while ((1 << l) <= a.deg()) ++l;
    vector<int> fa(1 << l);
    memcpy(fa.data(), a.a.data(), sizeof(int) * (a.deg() + 1));
    ntt.fft(fa.data(), l, 1);
    vector<int> res1 = _mul(l, fa, b1), res2 = _mul(l, fa, b2);
    res1.resize(a.deg() - b1.deg() + 1);
    res2.resize(a.deg() - b2.deg() + 1);
    return make_pair(res1, res2);
  }

  vector<int> ls, rs, pos;
  vector<Poly> p, q;

  void _build(int n) {
    ls.assign(n * 2 - 1, 0);
    rs.assign(n * 2 - 1, 0);
    p.assign(n * 2 - 1, 0);
    q.assign(n * 2 - 1, 0);
    pos.resize(n);
    int cnt = 0;
    function<int(int, int)> dfs = [&](int l, int r) {
      if (l == r) {
        pos[l] = cnt;
        return cnt++;
      }
      int ret = cnt++;
      int mid = (l + r) >> 1;
      ls[ret] = dfs(l, mid);
      rs[ret] = dfs(mid + 1, r);
      return ret;
    };
    dfs(0, n - 1);
  }

  vector<int> _eval(vector<int> f, const vector<int> &x) {
    int n = f.size();
    _build(n);
    for (int i = 0; i < n; ++i)
      q[pos[i]] = {1, norm(P - x[i])};
    for (int i = n * 2 - 2; i >= 0; --i)
      if (ls[i])
        q[i] = q[ls[i]] * q[rs[i]];
    f.resize(n * 2);
    p[0] = mul(f, q[0].inv());
    for (int i = 0; i < n * 2 - 1; ++i)
      if (ls[i])
        tie(p[ls[i]], p[rs[i]]) = mul2(p[i], q[rs[i]], q[ls[i]]);
    vector<int> ret(n);
    for (int i = 0; i < n; ++i)
      ret[i] = p[pos[i]][0];
    return ret;
  }

  vector<int> eval(const Poly &f, const vector<int> &x) {
    int n = f.deg() + 1, m = x.size();
    vector<int> tmpf = f.a, tmpx = x;
    tmpf.resize(max(n, m));
    tmpx.resize(max(n, m));
    vector<int> ret = _eval(tmpf, tmpx);
    ret.resize(m);
    return ret;
  }

  Poly inter(const vector<int> &x, const vector<int> &y) {
    int n = x.size();
    _build(n);
    for (int i = 0; i < n; ++i)
      q[pos[i]] = {1, norm(P - x[i])};
    for (int i = n * 2 - 2; i >= 0; --i)
      if (ls[i])
        q[i] = q[ls[i]] * q[rs[i]];
    Poly tmp = q[0];
    reverse(tmp.a.begin(), tmp.a.end());
    vector<int> f = tmp.der().a;
    f.resize(n * 2);
    p[0] = mul(f, q[0].inv());
    for (int i = 0; i < n * 2 - 1; ++i)
      if (ls[i])
        tie(p[ls[i]], p[rs[i]]) = mul2(p[i], q[rs[i]], q[ls[i]]);
    for (int i = 0; i < n; ++i)
      p[pos[i]] = nt.inv(p[pos[i]][0]) * (ll) y[i] % P;
    for (int i = 0; i < n * 2 - 1; ++i)
      reverse(q[i].a.begin(), q[i].a.end());
    for (int i = n * 2 - 2; i >= 0; --i)
      if (ls[i])
        p[i] = p[ls[i]] * q[rs[i]] + p[rs[i]] * q[ls[i]];
    return p[0];
  }

} tp;

Poly operator "" _z(unsigned long long a) { return {0, (int) a}; }

Poly operator+(int v, const Poly &rhs) { return Poly(v) + rhs; }

Poly Poly::operator*(const Poly &rhs) const {
  int n = deg(), m = rhs.deg();
  if (n <= 10 || m <= 10 || n + m <= BRUTE_N2_LIMIT) {
    Poly ret = zeroes(n + m);
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= m; ++j)
        ret[i + j] = (ret[i + j] + a[i] * (ll) rhs[j]) % P;
    return ret;
  }
  n += m;
  int l = 0;
  while ((1 << l) <= n)
    ++l;
  vector<int> res(1 << l), tmp(1 << l);
  memcpy(res.data(), base(), a.size() * sizeof(int));
  ntt.fft(res.data(), l, 1);
  memcpy(tmp.data(), rhs.base(), rhs.a.size() * sizeof(int));
  ntt.fft(tmp.data(), l, 1);
  for (int i = 0; i < (1 << l); ++i)
    res[i] = res[i] * (ll) tmp[i] % P;
  ntt.fft(res.data(), l, -1);
  res.resize(n + 1);
  return res;
}

Poly Poly::inv() const {
  Poly g = nt.inv(a[0]);
  for (int t = 0; (1 << t) <= deg(); ++t)
    nit.inv(slice((2 << t) - 1), g, t);
  g.redeg(deg());
  return g;
}

Poly Poly::taylor(int k) const {
  int n = deg();
  Poly t = zeroes(n);
  simp.check(n);
  for (int i = 0; i <= n; ++i)
    t[n - i] = a[i] * (ll) simp.fac[i] % P;
  int pw = 1;
  Poly help = vector<int>(simp.ifac.begin(), simp.ifac.begin() + n + 1);
  for (int i = 0; i <= n; ++i) {
    help[i] = help[i] * (ll) pw % P;
    pw = pw * (ll) k % P;
  }
  t = t * help;
  for (int i = 0; i <= n; ++i)
    help[i] = t[n - i] * (ll) simp.ifac[i] % P;
  return help;
}

Poly Poly::pow(int k) const {
  if (k == 0)
    return 1;
  if (k == 1)
    return *this;
  int n = deg() * k;
  int lgn = 0;
  while ((1 << lgn) <= n)
    ++lgn;
  vector<int> val = a;
  val.resize(1 << lgn);
  ntt.fft(val.data(), lgn, 1);
  for (int i = 0; i < (1 << lgn); ++i)
    val[i] = mpow(val[i], k);
  ntt.fft(val.data(), lgn, -1);
  return val;
}

Poly Poly::der() const {
  if (deg() == 0)
    return 0;
  vector<int> res(deg());
  for (int i = 0; i < deg(); ++i)
    res[i] = a[i + 1] * (ll) (i + 1) % P;
  return res;
}

Poly Poly::integ() const {
  vector<int> res(deg() + 2);
  simp.check(deg() + 1);
  for (int i = 0; i <= deg(); ++i)
    res[i + 1] = a[i] * (ll) simp.inv[i + 1] % P;
  return res;
}

Poly Poly::quo(const Poly &rhs) const {
  if (rhs.deg() == 0)
    return a[0] * (ll) nt.inv(rhs[0]) % P;
  Poly g = nt.inv(rhs[0]);
  int t = 0, n;
  for (n = 1; (n << 1) <= rhs.deg(); ++t, n <<= 1)
    nit.inv(rhs.slice((n << 1) - 1), g, t);
  Poly nttg = g;
  nttg.redeg((n << 1) - 1);
  ntt.fft(nttg.base(), t + 1, 1);
  Poly eps1 = rhs.slice((n << 1) - 1);
  ntt.fft(eps1.base(), t + 1, 1);
  for (int i = 0; i < (n << 1); ++i)
    eps1[i] = eps1[i] * (ll) nttg[i] % P;
  ntt.fft(eps1.base(), t + 1, -1);
  memcpy(eps1.base(), eps1.base() + n, sizeof(int) << t);
  memset(eps1.base() + n, 0, sizeof(int) << t);
  ntt.fft(eps1.base(), t + 1, 1);
  Poly h0 = slice(n - 1);
  h0.redeg((n << 1) - 1);
  ntt.fft(h0.base(), t + 1);
  Poly h0g0 = zeroes((n << 1) - 1);
  for (int i = 0; i < (n << 1); ++i)
    h0g0[i] = h0[i] * (ll) nttg[i] % P;
  ntt.fft(h0g0.base(), t + 1, -1);
  Poly h0eps1 = zeroes((n << 1) - 1);
  for (int i = 0; i < (n << 1); ++i)
    h0eps1[i] = h0[i] * (ll) eps1[i] % P;
  ntt.fft(h0eps1.base(), t + 1, -1);
  for (int i = 0; i < n; ++i) {
    h0eps1[i] = operator[](i + n) - h0eps1[i];
    if (h0eps1[i] < 0)
      h0eps1[i] += P;
  }
  memset(h0eps1.base() + n, 0, sizeof(int) << t);
  ntt.fft(h0eps1.base(), t + 1);
  for (int i = 0; i < (n << 1); ++i)
    h0eps1[i] = h0eps1[i] * (ll) nttg[i] % P;
  ntt.fft(h0eps1.base(), t + 1, -1);
  memcpy(h0eps1.base() + n, h0eps1.base(), sizeof(int) << t);
  memset(h0eps1.base(), 0, sizeof(int) << t);
  return (h0g0 + h0eps1).slice(rhs.deg());
}

Poly Poly::ln() const {
  if (deg() == 0)
    return 0;
  return der().quo(slice(deg() - 1)).integ();
}

pair<Poly, Poly> Poly::sqrti() const {
  Poly g = nt.sqrt(a[0]), h = nt.inv(g[0]), nttg = g;
  for (int t = 0; (1 << t) <= deg(); ++t) {
    nit.sqrt(slice((2 << t) - 1), g, nttg, h, t);
    nttg = g;
    ntt.fft(nttg.base(), t + 1, 1);
    nit.inv(g, nttg, h, t);
  }
  return make_pair(g.slice(deg()), h.slice(deg()));
}

Poly Poly::sqrt() const {
  Poly g = nt.sqrt(a[0]), h = nt.inv(g[0]), nttg = g;
  for (int t = 0; (1 << t) <= deg(); ++t) {
    nit.sqrt(slice((2 << t) - 1), g, nttg, h, t);
    if ((2 << t) <= deg()) {
      nttg = g;
      ntt.fft(nttg.base(), t + 1, 1);
      nit.inv(g, nttg, h, t);
    }
  }
  return g.slice(deg());
}

Poly Poly::exp() const {
  vector<int> der(a), ret(a.size());
  for (int i = 0; i < a.size(); ++i)
    der[i] = der[i] * (ll) i % P;
  src.run(der, ret, [&](int i) {
    if (i == 0) ret[0] = 1;
    else ret[i] = ret[i] * (ll) simp.ginv(i) % P;
  });
  return ret;
}

pair<Poly, Poly> Poly::expi() const {
  Poly g = 1, h = 1, nttg = {1, 1};
  for (int t = 0; (1 << t) <= deg(); ++t) {
    nit.exp(slice((2 << t) - 1), g, nttg, h, t);
    nttg = g;
    nttg.redeg((4 << t) - 1);
    ntt.fft(nttg.base(), t + 2);
    Poly f2n = zeroes((2 << t) - 1);
    for (int i = 0; i < (2 << t); ++i)
      f2n[i] = nttg[i << 1];
    nit.inv(g, f2n, h, t);
  }
  return make_pair(g.slice(deg()), h.slice(deg()));
}

Poly Poly::exp(int k) const {
  int lead, lz = 0;
  while (lz < deg() && !a[lz])
    ++lz;
  if (lz == deg() && !a[lz])
    return *this;
  lead = a[lz];
  if (lz * (ll) k > deg())
    return zeroes(deg());
  Poly part = Poly(vector<int>(a.begin() + lz, a.begin() + deg() - lz * (k - 1) + 1)) * nt.inv(lead);
  part = (part.ln() * k).exp() * mpow(lead, k);
  vector<int> ret(deg() + 1);
  memcpy(ret.data() + lz * k, part.base(), sizeof(int) * (deg() - lz * k + 1));
  return ret;
}

Poly Poly::operator/(const Poly &rhs) const {
  int n = deg(), m = rhs.deg();
  if (n < m)
    return 0;
  Poly ta(vector<int>(a.rbegin(), a.rend())),
          tb(vector<int>(rhs.a.rbegin(), rhs.a.rend()));
  ta.redeg(n - m);
  tb.redeg(n - m);
  Poly q = ta.quo(tb);
  reverse(q.a.begin(), q.a.end());
  return q;
}

pair<Poly, Poly> Poly::div(const Poly &rhs) const {
  if (deg() < rhs.deg())
    return make_pair(0, *this);
  int n = deg(), m = rhs.deg();
  Poly q = operator/(rhs), r;
  int lgn = 0;
  while ((1 << lgn) < rhs.deg())
    ++lgn;
  int t = (1 << lgn) - 1;
  r = zeroes(t);
  Poly tmp = zeroes(t);
  for (int i = 0; i <= m; ++i)
    if ((r[i & t] += rhs[i]) >= P)
      r[i & t] -= P;
  for (int i = 0; i <= n - m; ++i)
    if ((tmp[i & t] += q[i]) >= P)
      tmp[i & t] -= P;
  ntt.fft(r.base(), lgn, 1);
  ntt.fft(tmp.base(), lgn, 1);
  for (int i = 0; i <= t; ++i)
    tmp[i] = tmp[i] * (ll) r[i] % P;
  ntt.fft(tmp.base(), lgn, -1);
  memset(r.base(), 0, sizeof(int) << lgn);
  for (int i = 0; i <= n; ++i)
    if ((r[i & t] += a[i]) >= P)
      r[i & t] -= P;
  for (int i = 0; i < m; ++i)
    if ((r[i] -= tmp[i]) < 0)
      r[i] += P;
  return make_pair(q, r.slice(m - 1));
}

Poly Poly::operator%(const Poly &rhs) const {
  if (deg() < rhs.deg())
    return *this;
  return div(rhs).second;
}

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

const int N = 200010;

Poly solve(int n, int *a) {
  Poly prod;
  {
    function<Poly(int, int)> cprod = [&](int l, int r) {
      if (l == r) return Poly({a[l], 1});
      int mid = (l + r) >> 1;
      return cprod(l, mid) * cprod(mid + 1, r);
    };
    prod = cprod(1, n);
  }
  vector<int> points(n + 1);
  iota(points.begin(), points.end(), 0);
  Poly values = tp.eval(prod, points), ix = zeroes(n);
  for (int i = 0; i <= n; ++i) {
    values[i] = values[i] * (ull) simp.gifac(i) % P;
    ix[i] = (i & 1) ? (P - simp.gifac(i)) : simp.gifac(i);
  }
  values = (values * ix).slice(n);
  reverse(values.a.begin(), values.a.end());
  return values;
}

int a[N], f[N * 2];

Poly countCirc(int n) {
  int h = n;
  while (a[h] < h) --h;
  if (h == 0) return Poly(1);
  int j = 0;
  for (int i = h; i; --i, ++j) {
    a[i] -= h;
    sub(a[i], j);
    f[i * 2 - 1] = norm(a[i] + P - 1);
    f[i * 2] = a[i];
  }
  return solve(h * 2, f).slice(n);
}

int w;
int v[N];

Poly gao(int l, int r) {
  int n = r - l;
  if (n == 0) return Poly(1);
  int p = l;
  for (int i = r - 1; i >= l; --i) {
    while (p != r && v[p] * (ll)v[i] > w) ++p;
    a[i - l + 1] = p - l;
  }
  return countCirc(n);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n >> w;
  for (int i = 1; i <= n; ++i) cin >> v[i];
  sort(v + 1, v + n + 1);
  int lb = lower_bound(v + 1, v + n + 1, 0) - v;
  reverse(v + lb, v + n + 1);
  Poly pos = gao(lb, n + 1);
  for (int i = 1; i != lb; ++i) v[i] *= -1;
  Poly neg = gao(1, lb);
  Poly tot = pos * neg;
  int ans = 0;
  for (int i = 0; i <= n; ++i) {
    int res = tot[i] * (ll)simp.gfac(n - i) % P;
    if (i & 1) sub(ans, res);
    else add(ans, res);
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
