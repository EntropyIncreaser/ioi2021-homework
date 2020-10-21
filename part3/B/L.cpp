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
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
 
#include <algorithm>
#include <iomanip>
#include <tuple>
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

using pir = pair<double, ll>;

const int N = 44;

int n;
ll C[N][N];

ll MC(int a, int b, int c, int d) {
  return C[n][a] * C[n - a][b] * C[n - a - b][c];
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  double a, b, c, d;
  cin >> n >> a >> b >> c >> d;
  double ans = 0;
  for (int i = 0; i <= n; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j) {
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }
  }
  priority_queue<pir, vector<pir>, greater<pir>> pq;
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= n - i; ++j) {
      for (int k = 0; k <= n - i - j; ++k) {
        pq.emplace(pow(a, i) * pow(b, j) * pow(c, k) * pow(d, n - i - j - k), MC(i, j, k, n - i - j - k));
      }
    }
  }
  while (true) {
    pir x = pq.top(); pq.pop();
    if (x.second > 1) {
      ans += x.second / 2 * 2 * x.first;
      pq.emplace(x.first * 2, x.second / 2);
      x.second %= 2;
      if (x.second) pq.emplace(x);
    } else {
      if (pq.empty()) {
        break;
      }
      pir y = pq.top(); pq.pop();
      ans += x.first + y.first;
      pq.emplace(x.first + y.first, 1);
      --y.second;
      if (y.second) pq.emplace(y);
    }
  }
  cout << fixed << setprecision(10) << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
