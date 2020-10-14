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

const int N = 100010;

int n;
vector<int> g[N];
int dfn[N], dfnr[N], rev[N], dep[N];
int top[N], sub[N], pref[N], prt[N];
int cnt[N], pot[N];
set<int> st;

void dfs(int u) {
  static int t;
  if (!pot[u] && u != 1) pot[u] = u;
  dfn[u] = ++t;
  rev[t] = u;
  sub[u] = g[u].empty();
  for (int v : g[u]) {
    dep[v] = dep[u] + 1;
    pot[v] = pot[u];
    dfs(v);
    sub[u] += sub[v];
    if (sub[v] > sub[pref[u]])
      pref[u] = v;
  }
  dfnr[u] = t;
}

void hld(int u) {
  if (!top[u]) {
    top[u] = u;
  }
  if (pref[u]) {
    top[pref[u]] = top[u];
  }
  for (int v : g[u]) {
    prt[v] = u;
    hld(v);
  }
}

int lca(int u, int v) {
  int x = top[u], y = top[v];
  while (x != y) {
    if (dep[x] > dep[y]) {
      swap(x, y);
      swap(u, v);
    }
    v = prt[y];
    y = top[v];
  }
  return dep[u] < dep[v] ? u : v;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("gangsters.in", "r", stdin);
  freopen("gangsters.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int q;
  cin >> n >> q;
  for (int i = 2; i <= n; ++i) {
    int p;
    cin >> p;
    g[p].push_back(i);
  }
  dfs(1);
  hld(1);
  int ans1 = 0, ans2 = 0;
  auto apply = [&](int u, int sgn) {
    if (cnt[u]) {
      ans1 += sgn;
      int dl = *st.lower_bound(dfn[u]);
      int dr = *prev(st.upper_bound(dfnr[u]));
      ans2 += sgn * sub[lca(rev[dl], rev[dr])];
    }
  };
  while (q--) {
    cin.ignore();
    char c;
    int v;
    cin >> c >> v;
    apply(pot[v], -1);
    if (c == '+') {
      --ans2;
      ++cnt[pot[v]];
      st.insert(dfn[v]);
    } else {
      ++ans2;
      --cnt[pot[v]];
      st.erase(dfn[v]);
    }
    apply(pot[v], 1);
    cout << ans1 << ' ' << ans2 << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

