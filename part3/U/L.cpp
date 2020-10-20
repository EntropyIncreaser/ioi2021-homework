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

const int N = 100010;

int n, m;
int xn[N], y[N], pn[N];
int xm[N], z[N], pm[N];

int disc[N * 2];
double s[N * 2][2], t[N * 2][2];
double u[N * 2][4];

void integ(double arr[], int len) {
  for (int i = len + 1; i; --i) arr[i] = arr[i - 1] / i;
  arr[0] = 0;
}

double eval(double arr[], double x, int len) {
  double ret = 0;
  for (int i = len; i >= 0; --i) ret = ret * x + arr[i];
  return ret;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("lonely.in", "r", stdin);
  freopen("lonely.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < n; ++i) cin >> xn[i] >> y[i];
  cin >> m;
  for (int i = 0; i < m; ++i) cin >> xm[i] >> z[i];
  if (*max_element(y, y + n) != *max_element(z, z + m)) {
    cout << "Invalid plan\n";
    return 0;
  }
  copy_n(y, n, disc + 1);
  copy_n(z, m, disc + n + 1);
  sort(disc + 1, disc + n + m + 1);
  for (int i = 0; i < n; ++i) pn[i] = lower_bound(disc, disc + n + m + 1, y[i]) - disc;
  for (int i = 0; i < m; ++i) pm[i] = lower_bound(disc, disc + n + m + 1, z[i]) - disc;

  for (int i = 1; i < n; ++i) {
    if (y[i] == y[i - 1]) {
      s[pn[i]][0] += xn[i] - xn[i - 1];
    } else {
      double k = double(xn[i] - xn[i - 1]) / (y[i] - y[i - 1]);
      s[pn[i]][1] -= k;
      s[pn[i]][0] += k * y[i];
      s[pn[i - 1]][1] += k;
      s[pn[i - 1]][0] -= k * y[i - 1];
    }
  }

  for (int i = 1; i < m; ++i) {
    if (z[i] == z[i - 1]) {
      t[pm[i]][0] += xm[i] - xm[i - 1];
    } else {
      double k = double(xm[i] - xm[i - 1]) / (z[i] - z[i - 1]);
      t[pm[i]][1] -= k;
      t[pm[i]][0] += k * z[i];
      t[pm[i - 1]][1] += k;
      t[pm[i - 1]][0] -= k * z[i - 1];
    }
  }

  for (int i = n + m; i; --i) {
    s[i][0] += s[i + 1][0];
    s[i][1] += s[i + 1][1];
    t[i][0] += t[i + 1][0];
    t[i][1] += t[i + 1][1];
  }

  double ans = 0;
  for (int i = n + m; i; --i) {
    for (int j = 0; j <= 1; ++j)
      for (int k = 0; k <= 1; ++k)
        u[i][j + k] += s[i][j] * t[i][k];
    integ(u[i], 2);
    ans += eval(u[i], disc[i], 3) - eval(u[i], disc[i - 1], 3);
  }
  cout << fixed << setprecision(10) << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

