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

struct cd {
  double re, im;

  cd(double re = 0, double im = 0) : re(re), im(im) {}

  inline double real() const { return re; }
  inline double imag() const { return im; }

  inline void real(double re) { this->re = re; }
  inline void imag(double im) { this->im = im; }

  inline double norm() const { return re * re + im * im; }

  inline cd operator+(const cd &rhs) const { return cd(re + rhs.re, im + rhs.im); }
  inline cd operator-(const cd &rhs) const { return cd(re - rhs.re, im - rhs.im); }
  inline cd operator-() const { return cd(-re, -im); }
  inline cd operator*(const cd &rhs) const { return cd(re * rhs.re - im * rhs.im, re * rhs.im + im * rhs.re); }
  inline cd operator/(double x) const { return cd(re / x, im / x); }

  inline void operator+=(const cd &rhs) {
    re += rhs.re;
    im += rhs.im;
  }
  inline void operator/=(double x) {
    re /= x;
    im /= x;
  }
  inline void operator*=(const cd &rhs) { *this = *this * rhs; }

  inline friend cd conj(const cd &z) { return cd(z.re, -z.im); }

  inline cd operator/(const cd &rhs) const { return (*this * conj(rhs)) / rhs.norm(); }
};

const int BASE = 4, MOD = 10000, LGM = 17;
const double PI = 3.1415926535897932384626;

namespace ConvHelper {

struct FFT {
  int L;
  int brev[1 << 20];
  cd w[1 << 20];

  FFT() {}

  void init(int L) {
    this->L = L;
    for (int i = 0; i < (1 << L); ++i)
      brev[i] = (brev[i >> 1] >> 1) | ((i & 1) << (L - 1));
    for (int i = 0; i < 1 << L; ++i)
      w[i] = cd(cos(i * PI * 2 / (1 << L)), sin(i * PI * 2 / (1 << L)));
  }

  void dft(cd *a, int lgn, int d) {
    int n = 1 << lgn;
    for (int i = 0; i < n; ++i) {
      int rv = brev[i] >> (L - lgn);
      if (rv < i)
        swap(a[rv], a[i]);
    }
    int fa = L;
    for (int t = 1; t < n; t <<= 1) {
      --fa;
      for (int i = 0; i < n; i += t << 1) {
        cd *p = a + i;
        for (int j = 0; j < t; ++j) {
          cd x = p[j + t] * w[j << fa];
          p[j + t] = p[j] - x;
          p[j] += x;
        }
      }
    }
    if (d == -1) {
      reverse(a + 1, a + n);
      for (int i = 0; i < n; ++i)
        a[i] /= n;
    }
  }

  // realSeq FFT
  void dft(cd *a, cd *b, int lgn, int d) {
    int n = 1 << lgn;
    for (int i = 0; i < n; ++i)
      a[i].imag(b[i].real());
    dft(a, lgn, d);
    b[0] = conj(a[0]);
    for (int i = 1; i < n; ++i)
      b[i] = conj(a[n - i]);
    for (int i = 0; i < n; ++i) {
      cd x = a[i], y = b[i];
      a[i] = (x + y) / 2.;
      b[i] = (x - y) / cd(0, 2);
    }
  }

} fft;

vector<ll> conv(const vector<int> &a, const vector<int> &b) {
  int n = a.size() - 1, m = b.size() - 1;
  if (n < 100 / (m + 1) || n < 3 || m < 3) {
    vector<ll> ret(n + m + 1);
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= m; ++j)
        ret[i + j] += a[i] * (ll) b[j];
    return ret;
  }
  int lgn = 0;
  while ((1 << lgn) <= n + m)
    ++lgn;
  vector<cd> ta(a.begin(), a.end()), tb(b.begin(), b.end());
  ta.resize(1 << lgn);
  tb.resize(1 << lgn);

  if (a == b) {
    fft.dft(ta.begin().base(), lgn, 1);
    tb = ta;
  } else
    fft.dft(ta.begin().base(), tb.begin().base(), lgn, 1);
  for (int i = 0; i < (1 << lgn); ++i)
    ta[i] *= tb[i];
  fft.dft(ta.begin().base(), lgn, -1);
  vector<ll> ret(n + m + 1);
  for (int i = 0; i <= n + m; ++i)
    ret[i] = ta[i].real() + 0.5;
  return ret;
}

}

const int N = 500010;

int d[N];
ll ans[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 1; i < N; ++i)
    for (int j = i; j < N; j += i)
      ++d[j];
  ConvHelper::fft.init(20);
  auto res = ConvHelper::conv(vector<int>(d, d + N), vector<int>(d, d + N));
  for (int i = 1; i < N; ++i) ans[i] = res[i];
  int q;
  cin >> q;
  while (q--) {
    int l, r;
    cin >> l >> r;
    int pos = max_element(ans + l, ans + r + 1) - ans;
    cout << pos << ' ' << ans[pos] << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

