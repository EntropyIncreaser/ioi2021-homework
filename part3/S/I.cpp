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
int a[N], b[N], c[N], d[N];
int ans[N * 2], pos[N], ord[N];
bool vis[N];
vector<int> g[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("insider.in", "r", stdin);
  freopen("insider.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 1; i <= m; ++i) {
    cin >> a[i] >> b[i] >> c[i];
    g[a[i]].push_back(i);
    g[c[i]].push_back(i);
    ++d[b[i]];
  }
  queue<int> q;
  for (int i = 1; i <= n; ++i)
    if (!d[i]) q.push(i);
  int cnt = 0;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    ord[++cnt] = x;
    for (int y : g[x])
      if (!vis[y]) {
        vis[y] = true;
        if (!--d[b[y]]) q.push(b[y]);
      }
  }
  int L = n, R = n;
  for (int rep = n; rep; --rep) {
    int i = ord[rep];
    int lc = 0, rc = 0;
    for (int j : g[i]) {
      int x = a[j], y = c[j];
      if (x != i) swap(x, y);
      if (pos[b[j]] < pos[y]) ++lc;
      else ++rc;
    }
    if (lc >= rc) ans[pos[i] = --L] = i;
    else ans[pos[i] = R++] = i;
  }
  for (int i = L; i != R; ++i)
    cout << ans[i] << " \n"[i == R - 1];

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

