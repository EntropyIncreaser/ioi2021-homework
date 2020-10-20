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

const int N = 510;

int m;
int a[7], b[N];
int dig[4];
int dp[3][N][N];

template<class T>
void checkMin(T &x, const T &y) {
  if (y < x) {
    x = y;
  }
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    int v;
    cin >> v;
    if (v % 1111111 == 0) {
      continue;
    }
    ++m;
    static int k[7];
    k[0] = v;
    for (int j = 1; j < 7; ++j) {
      k[j] = k[j - 1] * 10 - k[j - 1] / 1000000 * 9999999;
    }
    b[m] = max_element(k, k + 7) - k;
  }
  for (int i = 0; i <= m; ++i) {
    ++a[(7 + b[i] - b[i + 1]) % 7];
  }
  int ans = 0;
  for (int i = 1; i <= 3; ++i) {
    int u = min(a[i], a[7 - i]);
    ans += u;
    a[i] -= u;
    a[7 - i] -= u;
    if (a[7 - i]) {
      swap(a[i], a[7 - i]);
      dig[i] = 7 - i;
    } else {
      dig[i] = i;
    }
  }
  for (int i = 0; i <= a[1]; ++i) {
    for (int j = 0; j <= a[2]; ++j) {
      for (int k = 0; k <= a[3]; ++k) {
        dp[i % 3][j][k] = 1e9;
        if (i == 0 && j == 0 && k == 0) {
          dp[i % 3][j][k] = 0;
        }
        if ((i * dig[1] + j * dig[2] + k * dig[3]) % 7) {
          if (i) {
            checkMin(dp[i % 3][j][k], dp[(i - 1) % 3][j][k] + 1);
          }
          if (j) {
            checkMin(dp[i % 3][j][k], dp[i % 3][j - 1][k] + 1);
          }
          if (k) {
            checkMin(dp[i % 3][j][k], dp[i % 3][j][k - 1] + 1);
          }
        } else {
          if (i && j) {
            checkMin(dp[i % 3][j][k], dp[(i - 1) % 3][j - 1][k] + 1);
          }
          if (j && k) {
            checkMin(dp[i % 3][j][k], dp[i % 3][j - 1][k - 1] + 1);
          }
          if (k && i) {
            checkMin(dp[i % 3][j][k], dp[(i - 1) % 3][j][k - 1] + 1);
          }
          if (i >= 2) {
            checkMin(dp[i % 3][j][k], dp[(i - 2) % 3][j][k] + 1);
          }
          if (j >= 2) {
            checkMin(dp[i % 3][j][k], dp[i % 3][j - 2][k] + 1);
          }
          if (k >= 2) {
            checkMin(dp[i % 3][j][k], dp[i % 3][j][k - 2] + 1);
          }
        }
      }
    }
  }
  cout << dp[a[1] % 3][a[2]][a[3]] + ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

