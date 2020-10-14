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

typedef double db;

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

const int K = 45, Base = 100000000;

struct Int {
  int digit[K];

  Int(int x = 0) {
    memset(digit, 0, sizeof(digit));
    digit[0] = x;
  }

  Int(const Int &v) { memcpy(digit, v.digit, sizeof(digit)); }

  Int operator+(const Int &rhs) const {
    Int ret = Int();
    for (int i = 0; i < K; ++i) ret.digit[i] = digit[i] + rhs.digit[i];
    for (int i = 0; i < K; ++i) {
      if (ret.digit[i] >= Base) {
        ++ret.digit[i + 1];
        ret.digit[i] -= Base;
      }
    }
    return ret;
  }

  void operator+=(const Int &rhs) {
    for (int i = 0; i < K; ++i) {
      if ((digit[i] += rhs.digit[i]) >= Base) {
        ++digit[i + 1];
        digit[i] -= Base;
      }
    }
  }

  void operator-=(const Int &rhs) {
    for (int i = 0; i < K; ++i) {
      if ((digit[i] -= rhs.digit[i]) < 0) {
        digit[i] += Base;
        --digit[i + 1];
      }
    }
  }

  Int operator*(const Int &rhs) const {
    Int ret = Int();
    unsigned int carry = 0;
    for (int i = 0; i < K; ++i) {
      ull v = carry;
      for (int j = 0; j <= i; ++j) {
        v += digit[j] * (ull) rhs.digit[i - j];
      }
      carry = v / Base;
      ret.digit[i] = v - carry * (ull) Base;
    }
    return ret;
  }

  db toDb() const {
    db ret = 0;
    for (int i = K - 1; i >= 0; --i)
      ret = ret * Base + digit[i];
    return ret;
  }
};

const int N = 510;

db C[N * 2][N * 2];
Int iC[N * 2][N];

void prepare(int n) {
  for (int i = 0; i <= n; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j)
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
  }
  for (int i = 0; i <= n; ++i) {
    iC[i][0] = Int(1);
    for (int j = 1; j <= min(i, (n + 1) / 2); ++j) {
      iC[i][j] = iC[i - 1][j - 1] + iC[i - 1][j];
    }
  }
}

Int negBinom(int k, int n) {
  if (k == 0) return n == 0;
  return iC[n + k - 1][min(n, k - 1)];
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, d, r;
  cin >> n >> d >> r;
  prepare(n + d);
  db ans = 0;
  for (int i = 0; i < d; ++i) {
    for (int j = 1; j <= min(n, d / (i + 1)); ++j) {
      Int padd = Int(), psub = Int();
      for (int k = 0; k <= min(n, d / (i + 1)) - j; ++k) {
        Int p = negBinom(n, d - (j + k) * (i + 1)) * iC[n - j][min(k, n - j - k)];
        if (k & 1) psub += p;
        else padd += p;
      }
      padd -= psub;
      db totp = padd.toDb() * C[n][j];
      ans += totp * min(j, r);
    }
  }
  ans /= C[n + d - 1][d];
  ans += r;
  cout << fixed << setprecision(10) << (double) ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (db)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
