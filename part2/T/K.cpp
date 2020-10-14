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

const int Mod = 1000000007;

const int N = 1010, T = 110, Q = 255;

void add(int& x, int y) {
  if ((x += y) >= Mod)
    x -= Mod;
}

int dp[T][Q];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("kebab.in", "r", stdin);
  freopen("kebab.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, t;
  cin >> n >> t;
  int ans = 1;
  int p = 0;
  auto flush = [&](int q) {
    for (int i = 0; i < T; ++i) {
      int val = accumulate(dp[i], dp[i] + Q, 0ull) % Mod;
      memset(dp[i], 0, sizeof(dp[i]));
      dp[i][q] = val;
    }
  };
  while (n--) {
    int q, x;
    cin >> q >> x;
    flush(q - x);
    for (int rep = 0; rep < q; ++rep) {
      memcpy(dp[(p + 1) % T], dp[p], sizeof(dp[p]));
      int prev = (p - t + T) % T;
      for (int i = 1; i < Q; ++i) {
        add(dp[(p + 1) % T][i - 1], dp[prev][i]);
        add(ans, dp[prev][i]);
      }
      p = (p + 1) % T;
      if (q > x) {
        add(dp[p][q - x - 1], 1);
        add(ans, 1);
      }
    }
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

