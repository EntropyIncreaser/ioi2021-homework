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

int n, q;
int cnt[N];
int ans[N];
int dis[N];
vector<int> G[N];

void solve(const vector<int> &nodes, const vector<pair<int, int>> &edges, const vector<tuple<int, int, int>> &queries) {
  int c = 0, n1 = nodes.size();
  for (int u : nodes) {
    cnt[u] = ++c;
  }
  int dist = numeric_limits<int>::max(), eL = 0, eR = 0;
  for (const auto&[u, v] : edges) {
    int cdist = abs(n1 - 2 * (cnt[v] - cnt[u]));
    if (cdist < dist) {
      dist = cdist;
      eL = u;
      eR = v;
    }
    G[u].push_back(v);
    G[v].push_back(u);
  }
  auto side = [&](int u) {
    if (u == eL || u == eR) {
      return 0;
    }
    if (eL < u && u < eR) {
      return 1;
    }
    return -1;
  };
  auto upAns = [&](int u) {
    for (int v : nodes) {
      dis[v] = -1;
    }
    queue<int> que;
    dis[u] = 0;
    que.push(u);
    while (!que.empty()) {
      u = que.front();
      que.pop();
      for (int v : G[u]) {
        if (dis[v] == -1) {
          dis[v] = dis[u] + 1;
          que.push(v);
        }
      }
    }
    for (const auto&[qu, qv, id] : queries) {
      ans[id] = min(ans[id], dis[qu] + dis[qv]);
    }
  };
  upAns(eL);
  upAns(eR);
  vector<int> nodes0, nodes1;
  vector<pair<int, int>> edges0, edges1;
  vector<tuple<int, int, int>> queries0, queries1;
  for (int u : nodes) {
    G[u].clear();
    if (side(u) >= 0) {
      nodes1.push_back(u);
    }
    if (side(u) <= 0) {
      nodes0.push_back(u);
    }
  }
  for (const auto&[u, v] : edges) {
    if (side(u) >= 0 && side(v) >= 0) {
      edges1.emplace_back(u, v);
    }
    if (side(u) <= 0 && side(v) <= 0) {
      edges0.emplace_back(u, v);
    }
  }
  for (const auto&[qu, qv, id] : queries) {
    if (side(qu) == side(qv)) {
      if (side(qu) == 1) {
        queries1.emplace_back(qu, qv, id);
      } else if (side(qu) == -1) {
        queries0.emplace_back(qu, qv, id);
      }
    }
  }
  if (nodes.size() == 3) {
    return;
  }
  if (!edges0.empty()) {
    solve(nodes0, edges0, queries0);
  }
  if (!edges1.empty()) {
    solve(nodes1, edges1, queries1);
  }
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("distance.in", "r", stdin);
  freopen("distance.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  vector<pair<int, int>> edges;
  for (int i = 1; i < n; ++i) {
    edges.emplace_back(i, i + 1);
  }
  edges.emplace_back(1, n);
  for (int rep = 0; rep < n - 3; ++rep) {
    int u, v;
    cin >> u >> v;
    if (u > v) {
      swap(u, v);
    }
    edges.emplace_back(u, v);
  }
  cin >> q;
  vector<tuple<int, int, int>> queries;
  fill_n(ans + 1, q, numeric_limits<int>::max());
  for (int i = 1; i <= q; ++i) {
    int x, y;
    cin >> x >> y;
    queries.emplace_back(x, y, i);
    if (x == y) {
      ans[i] = 0;
    }
  }
  vector<int> nodes(n);
  iota(nodes.begin(), nodes.end(), 1);
  solve(nodes, edges, queries);
  for (int i = 1; i <= q; ++i) {
    cout << ans[i] << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
