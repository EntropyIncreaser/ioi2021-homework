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

struct Node {
  int u;
  ll step;

  Node(const int &u, const ll &step) : u(u), step(step) {}

  bool operator>(const Node &rhs) const { return step > rhs.step; }
};

const int N = 3010;

ll dis[N];
vector<pair<int, int>> g[N];
int w[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, k;
  cin >> n >> m >> k;
  for (int i = 1; i <= m; ++i) {
    int u, v;
    cin >> u >> v >> w[i];
    g[u].emplace_back(v, w[i]);
    g[v].emplace_back(u, w[i]);
  }
  sort(w, w + m + 1);
  ll ans = numeric_limits<ll>::max();
  for (int i = 0; i <= m; ++i) {
    memset(dis, -1, sizeof(dis));
    priority_queue<Node, vector<Node>, greater<Node>> q;
    q.emplace(1, dis[1] = 0);
    while (!q.empty()) {
      auto t = q.top();
      q.pop();
      if (t.step != dis[t.u])continue;
      if (t.u == n)break;
      for (auto[v, z]:g[t.u]) {
        z = max(0, z - w[i]);
        if (dis[v] == -1 || dis[v] > t.step + z)q.emplace(v, dis[v] = t.step + z);
      }
    }
    ans = min(ans, dis[n] + k * (ll) w[i]);
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
