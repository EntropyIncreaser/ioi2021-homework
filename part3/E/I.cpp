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

const int N = 3010, M = 4510;

bool vis[N][M][3];
int R[N][M];
int L[N], U[N], D[N];

struct FW {
  int a[M];
  void ch(int k, int x) {
    for (; k < M; k += k & -k) a[k] += x;
  }
  int qry(int k) const {
    int ret = 0;
    for (; k; k &= k - 1) ret += a[k];
    return ret;
  }
} fw[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int r, c;
  cin >> r >> c;
  int n = r, m = 0;
  cin.ignore();
  for (int i = 1; i <= r; ++i) {
    string s;
    getline(cin, s);
    for (int j = 0; j < c; ++j)
      if ((i + j) % 2 == 1) {
        vis[i][(i + j + 1) / 2][0] = s[j * 2 + 1] == '-';
        m = max(m, (i + j + 1) / 2);
      }
    if (i == r) break;
    getline(cin, s);
    for (int j = 0; j < c; ++j)
      if ((i + j) % 2 == 1) {
        if (j * 2 + 1 < s.size())
          vis[i][(i + j + 1) / 2][2] = s[j * 2 + 1] == '\\';
        if (j * 2 > 0)
          vis[i][(i + j + 1) / 2][1] = s[j * 2 - 1] == '/';
      }
  }
  for (int i = 1; i <= n; ++i)
    for (int j = m; j; --j)
      if (vis[i][j][0])R[i][j] = R[i][j + 1] + 1;
  ll ans = 0;
  for (int j = 1; j <= m; ++j) {
    for (int i = 1; i <= n; ++i)
      if (!vis[i - 1][j - 1][2])
        for (int t = -1; t == -1 || vis[i + t][j + t][2]; ++t)
          fw[i + t + 1].ch(j + t + 1, 1);
    for (int i = 1; i <= n; ++i)
      if (vis[i][j - 1][0]) ++L[i];
      else L[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (vis[i - 1][j][1]) U[i] = U[i - 1] + 1;
      else U[i] = 0;
    for (int i = n; i; --i)
      if (vis[i][j][1]) D[i] = D[i + 1] + 1;
      else D[i] = 0;
    for (int i = 1; i <= n; ++i) {
      int ql = j - min(D[i], L[i]), qr = j + min(U[i], R[i][j]);
      ans += fw[i].qry(qr) - fw[i].qry(ql - 1) - 1;
    }
    for (int i = 1; i <= n; ++i)
      if (!vis[i][j][2])
        for (int t = 0; t == 0 || vis[i - t][j - t][2]; ++t)
          fw[i - t].ch(j - t, -1);
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

