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
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
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

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
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

int n, f;
int s[N], t[N];
int prt[N], top[N], dep[N], dis[N], l[N], r[N], tl[N], tr[N];
vector<int> g[N];
vector<int> adj[N];

void dfs(int u) {
  for (int v : g[u]) {
    if (!prt[v]) {
      prt[v] = u;
      dep[v] = dep[u] + 1;
      dfs(v);
    }
  }
}

int find(int x) {
  return top[x] == x ? x : top[x] = find(top[x]);
}

void GG() {
  cout << "No\n";
  exit(0);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> f;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  prt[1] = -1;
  dfs(1);
  iota(top + 1, top + n + 1, 1);
  for (int i = 1; i <= f; ++i) {
    cin >> s[i] >> t[i];
    int x = find(s[i]), y = find(t[i]);
    while (x != y) {
      if (dep[x] < dep[y]) {
        swap(x, y);
      }
      adj[x].push_back(prt[x]);
      adj[prt[x]].push_back(x);
      top[x] = prt[x];
      x = find(x);
    }
  }
  for (int i = 1; i <= n; ++i) {
    if (adj[i].size() > 2) {
      GG();
    }
  }
  for (int i = 1; i <= n; ++i) {
    if (adj[i].size() == 1) {
      int x = i;
      while (!adj[x].empty()) {
        int y = adj[x].back();
        dis[y] = dis[x] + 1;
        r[x] = y;
        l[y] = x;
        adj[y].erase(find(adj[y].begin(), adj[y].end(), x));
        adj[x].clear();
        x = y;
      }
    }
  }
  vector<pair<int, int>> paths;
  for (int i = 1; i <= f; ++i) {
    int d = abs(dis[s[i]] - dis[t[i]]);
    if (d) {
      paths.emplace_back(d, i);
    }
  }
  sort(paths.begin(), paths.end());
  iota(tl + 1, tl + n + 1, 1);
  iota(tr + 1, tr + n + 1, 1);
  for (const auto& [ig, id] : paths) {
    int x = s[id], y = t[id];
    if (dis[x] > dis[y]) {
      swap(x, y);
    }
    if (tl[x] != x || tr[y] != y) {
      GG();
    }
    while (tr[x] != y) {
      int rt = r[tr[x]];
      tl[rt] = x;
      tr[tr[x]] = tr[rt];
      tl[tr[rt]] = x;
      tr[x] = tr[rt];
    }
  }
  cout << "Yes\n";

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
