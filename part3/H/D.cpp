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

const int N = 1010;

namespace SSSP {

const int V = N, E = N * 5;

struct Ed {
  int v, w, c;
  Ed *next, *rev;
};

Ed *g[V], *top;
Ed pool[E * 2];

void clear() {
  memset(pool, 0, sizeof(pool));
  memset(g, 0, sizeof(g));
  top = pool;
}

Ed *adde(int u, int v, int w, int c) {
  Ed *p = top++;
  p->v = v;
  p->w = w;
  p->c = c;
  p->next = g[u];
  g[u] = p;
  return p;
}

Ed *link(int u, int v, int w, int c) {
  Ed *p = adde(u, v, w, c), *q = adde(v, u, 0, -c);
  p->rev = q;
  q->rev = p;
  return q;
}

bool vis[V], inq[V];
ll dis[V];
Ed *from[V];
queue<int> q;

void spfa(int s) {
  memset(vis, 0, sizeof(vis));
  vis[s] = true;
  dis[s] = 0;
  inq[s] = true;
  q.push(s);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    inq[u] = false;
    for (Ed *p = g[u]; p; p = p->next)
      if (p->w && (!vis[p->v] || dis[p->v] < dis[u] + p->c)) {
        vis[p->v] = true;
        from[p->v] = p;
        dis[p->v] = dis[u] + p->c;
        if (!inq[p->v]) {
          inq[p->v] = true;
          q.push(p->v);
        }
      }
  }
}

ll mcmf(int s, int t) {
  ll ret = 0;
  while (spfa(s), vis[t]) {
    ret += dis[t];
    int u = t;
    while (u != s) {
      --from[u]->w;
      ++from[u]->rev->w;
      u = from[u]->rev->v;
    }
  }
  return ret;
}

}

int n, k, l, r;
int s[N], e[N];
int deg[N];
SSSP::Ed *st[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("delight.in", "r", stdin);
  freopen("delight.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k >> r >> l;
  r = k - r;
  for (int i = 1; i <= n; ++i)cin >> s[i];
  for (int i = 1; i <= n; ++i)cin >> e[i];
  SSSP::clear();
  ll ans = 0;
  int m = n - k + 1;
  int S = m + 1, T = m + 2;
  for (int i = 1; i <= n; ++i) {
    e[i] -= s[i];
    ans += s[i];
    int x = min(i, m), y = max(0, i - k);
    st[i] = SSSP::link(x, y, 1, e[i]);
    if (e[i] > 0) {
      ans += e[i];
      ++st[i]->w;
      --st[i]->rev->w;
      ++deg[y];
      --deg[x];
    }
  }
  for (int i = 1; i <= m; ++i) {
    deg[i] += l;
    deg[i - 1] -= l;
    SSSP::link(i - 1, i, r - l, 0);
  }
  for (int i = 0; i <= m; ++i)
    if (deg[i] > 0)SSSP::link(S, i, deg[i], 0);
    else if (deg[i] < 0) SSSP::link(i, T, -deg[i], 0);
  ans += SSSP::mcmf(S, T);
  cout << ans << '\n';
  string str;
  for (int i = 1; i <= n; ++i)
    if (st[i]->w)str.push_back('E');
    else str.push_back('S');
  cout << str << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
