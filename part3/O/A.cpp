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

pair<int, vector<int>> maxMatching(vector<vector<int>> g) {
  g.emplace_back();
  int n = g.size();
  vector<int> match(n, -1);

  auto augment = [&](int i) {
    vector<int> color(n, -1), prt(n, -1), f(n), vis(n);
    queue<int> q;
    iota(f.begin(), f.end(), 0);
    function<int(int)> find = [&](int x) { return f[x] == x ? x : f[x] = find(f[x]); };
    auto unite = [&](int x, int y) { f[find(x)] = find(y); };
    int tt = 0;
    auto lca = [&](int x, int y) {
      ++tt;
      x = find(x);
      y = find(y);
      while (true) {
        if (x != -1) {
          if (vis[x] == tt) return x;
          vis[x] = tt;
          if (match[x] != -1)
            x = find(prt[match[x]]);
          else
            x = -1;
        }
        swap(x, y);
      }
    };
    auto blossom = [&](int x, int y, int base) {
      while (find(x) != base) {
        prt[x] = y;
        if (color[match[x]] == 1) {
          q.push(match[x]);
          color[match[x]] = 0;
        }
        if (find(x) == x)
          unite(x, base);
        if (find(match[x]) == match[x])
          unite(match[x], base);
        y = match[x];
        x = prt[y];
      }
    };
    q.push(i);
    color[i] = 0;
    while (!q.empty()) {
      int x = q.front();
      q.pop();
      for (auto y : g[x]) {
        if (color[y] == -1) {
          color[y] = 1;
          prt[y] = x;
          if (match[y] == -1) {
            while (x != -1) {
              int last = match[x];
              match[x] = y;
              match[y] = x;
              if (last != -1) {
                y = last;
                x = prt[y];
              } else break;
            }
            return;
          } else {
            q.push(match[y]);
            color[match[y]] = 0;
          }
        } else if (color[y] == 0 && find(x) != find(y)) {
          int base = lca(x, y);
          blossom(x, y, base);
          blossom(y, x, base);
        }
      }
    }
  };

  for (int i = 0; i < n; ++i)
    if (match[i] == -1)
      augment(i);

  vector<int> can(n - 1);
  auto tmp = match;
  int t0 = count(tmp.begin(), tmp.end(), -1);
  for (int i = 0; i < n - 1; ++i) {
    g[i].push_back(n - 1);
    g[n - 1].push_back(i);
    augment(n - 1);
    if (count(match.begin(), match.end(), -1) < t0) can[i] = 1;
    g[i].pop_back();
    g[n - 1].pop_back();
    match = tmp;
  }

  return make_pair((n - t0) / 2, can);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<vector<int>> g(n);
    for (int rep = 1; rep < n; ++rep) {
      int u, v;
      cin >> u >> v;
      --u;
      --v;
      g[u].push_back(v);
      g[v].push_back(u);
    }
    vector<vector<int>> e(n, vector<int>(n));
    int m;
    cin >> m;
    while (m--) {
      int u, v;
      cin >> u >> v;
      --u;
      --v;
      e[u][v] = e[v][u] = 1;
    }
    vector<int> vis(n);
    int ans = 0;
    function<vector<int>(int)> dfs = [&](int u) {
      vis[u] = 1;
      vector<int> can;
      vector<vector<int>> grp;
      can.push_back(u);
      for (int v : g[u])
        if (!vis[v]) {
          auto tmp = dfs(v);
          bool f = false;
          for (int x : tmp)
            if (e[u][x])
              f = true;
          if (f)
            ++ans;
          else
            grp.push_back(tmp);
        }
      if (!grp.empty()) {
        int n1 = grp.size();
        vector<vector<int>> g1(n1);
        for (int i = 1; i < n1; ++i)
          for (int j = 0; j < i; ++j) {
            bool fl = false;
            for (int x : grp[i])
              for (int y : grp[j])
                if (e[x][y])
                  fl = true;
            if (fl) {
              g1[i].push_back(j);
              g1[j].push_back(i);
            }
          }
        int enh;
        vector<int> ok;
        tie(enh, ok) = maxMatching(g1);
        ans += enh;
        for (int i = 0; i < n1; ++i)
          if (ok[i])
            for (int x : grp[i])
              can.push_back(x);
      }
      return can;
    };
    dfs(0);
    cout << ans << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
