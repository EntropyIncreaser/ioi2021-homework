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
  int step;

  Node(const int &u, const int &step) : u(u), step(step) {}

  bool operator>(const Node &rhs) const { return step > rhs.step; }
};

const int N = 5010, M = 50010;

int dis[N], val[N];
int eu[M], ev[M], ew[M];
ll dp[N][N];
vector<pair<int, int>> g[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, b, s, r;
  cin >> n >> b >> s >> r;
  for (int i = 1; i <= r; ++i) cin >> eu[i] >> ev[i] >> ew[i];
  for (int rep = 0; rep != 2; ++rep) {
    for (int i = 1; i <= n; ++i) g[i].clear();
    for (int i = 1; i <= r; ++i) {
      g[eu[i]].emplace_back(ev[i], ew[i]);
      swap(eu[i], ev[i]);
    }
    memset(dis, -1, sizeof(dis));
    priority_queue<Node, vector<Node>, greater<Node>> q;
    q.emplace(b + 1, dis[b + 1] = 0);
    while (!q.empty()) {
      auto t = q.top(); q.pop();
      if (t.step != dis[t.u]) continue;
      for (const auto& [v, w] : g[t.u])
        if (dis[v] == -1 || dis[v] > t.step + w)
          q.emplace(v, dis[v] = t.step + w);
    }
    for (int i = 1; i <= b; ++i) val[i] += dis[i];
  }
  sort(val + 1, val + b + 1);
  for (int i = 1; i <= b; ++i) {
    dp[i][0] = 1LL << 60;
    for (int j = 1; j <= s; ++j) {
      dp[i][j] = 1LL << 60;
      ll sum = 0;
      for (int k = 1; k <= i / j; ++k) {
        sum += val[i - k + 1];
        dp[i][j] = min(dp[i][j], dp[i - k][j - 1] + sum * (k - 1));
      }
    }
  }
  cout << dp[b][s] << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
