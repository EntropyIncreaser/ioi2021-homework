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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

ostream& operator<<(ostream& os, const pair<char, int>& unit) {
  return os << unit.first << "^" << unit.second;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

const int N = 10010;

int x[N], y[N];
int l[N], r[N];
ll dp[N];

ll sq(int v) { return v * (ll)v; }

int solve(int dx, int dy) {
  return 2 * (dx + dy) + sqrt(8LL * dx * dy);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, h, a, b;
  cin >> n >> h >> a >> b;
  for (int i = 1; i <= n; ++i) {
    cin >> x[i] >> y[i];
  }
  for (int i = 1; i <= n; ++i) {
    int range = numeric_limits<int>::max();
    r[i] = n;
    for (int j = i; j <= n; ++j) {
      if (x[j] - x[i] > range) {
        r[i] = j - 1;
        break;
      }
      range = min(range, solve(x[j] - x[i], h - y[j]));
    }
    range = numeric_limits<int>::max();
    l[i] = 1;
    for (int j = i; j; --j) {
      if (x[i] - x[j] > range) {
        l[i] = j + 1;
        break;
      }
      range = min(range, solve(x[i] - x[j], h - y[j]));
    }
  }
  dp[1] = a * (h - y[1]);
  for (int i = 2; i <= n; ++i) {
    dp[i] = 1LL << 60;
    for (int j = i - 1; j >= l[i]; --j) {
      if (i <= r[j]) {
        dp[i] = min(dp[i], dp[j] + sq(x[j] - x[i]) * b);
      }
    }
    dp[i] += a * (h - y[i]);
  }
  if (dp[n] >= (1LL << 60)) {
    cout << "impossible\n";
  } else {
    cout << dp[n] << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

