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

template<class Z>
struct MaxFlow {
  vector<vector<tuple<int, int, Z>>> g;

  struct Ref {
    const MaxFlow *p;
    int u, id;

    Ref() {}

    Ref(const MaxFlow *p, int u, int id) : p(p), u(u), id(id) {}

    Z get() const { return std::get<2>((p->g)[u][id]); }
  };

  MaxFlow() {}
  MaxFlow(int n) : g(n) {}

  Ref addEdge(int u, int v, const Z &w) {
    if (u == v) return Ref();
    int ru = g[u].size(), rv = g[v].size();
    g[u].emplace_back(v, rv, w);
    g[v].emplace_back(u, ru, 0);
    return Ref(this, v, rv);
  }

  vector<int> level;

  Z dinic(int s, int t) {
    int n = g.size();
    function<vector<int>()> getLevel = [&]() {
      vector<int> level(n, -1);
      queue<int> q;
      level[s] = 0;
      q.push(s);
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto &pr : g[u])
          if (get<2>(pr) && level[get<0>(pr)] == -1) {
            level[get<0>(pr)] = level[u] + 1;
            q.push(get<0>(pr));
          }
      }
      return level;
    };
    vector<int> r;
    function<Z(int, Z)> cap = [&](int u, Z limit) {
      if (u == t) return limit;
      Z ret = 0;
      while (r[u] && limit > ret) {
        int v = get<0>(g[u][r[u] - 1]), rev = get<1>(g[u][r[u] - 1]);
        Z &w = get<2>(g[u][r[u] - 1]);
        if (level[v] == level[u] + 1) {
          Z flow = cap(v, min(limit - ret, w));
          ret += flow;
          w -= flow;
          get<2>(g[v][rev]) += flow;
          if (limit == ret)
            return ret;
          --r[u];
        } else
          --r[u];
      }
      return ret;
    };
    Z ret = 0;
    while (level = getLevel(), level[t] != -1) {
      r = vector<int>(n);
      for (int i = 0; i < n; ++i)
        r[i] = g[i].size();
      ret += cap(s, numeric_limits<Z>::max());
    }
    return ret;
  }
};

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("heavy.in", "r", stdin);
  freopen("heavy.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, k;
  cin >> n >> k;
  vector<string> chains(n);
  cin >> chains;
  unordered_map<string, int> pre, suf;
  vector<int> x(n), y(n);
  int cp = 0, cs = 0;
  for (int i = 0; i < n; ++i) {
    string s = chains[i];
    if (!pre.count(s.substr(0, k)))
      pre.insert(make_pair(s.substr(0, k), cp++));
    x[i] = pre[s.substr(0, k)];
    if (!suf.count(s.substr(s.size() - k)))
      suf.insert(make_pair(s.substr(s.size() - k), cs++));
    y[i] = suf[s.substr(s.size() - k)];
  }
  MaxFlow<int> flow(cp + cs + 2);
  int S = 0, T = cp + cs + 1;
  for (int i = 0; i < cp; ++i)flow.addEdge(S, 1 + i, 1);
  for (int i = 0; i < cs; ++i)flow.addEdge(1 + cp + i, T, 1);
  for (int i = 0; i < n; ++i)
    flow.addEdge(1 + x[i], 1 + cp + y[i], numeric_limits<int>::max());
  int ans = flow.dinic(S, T);
  cout << ans << '\n';
  vector<vector<int>> vx(cp), vy(cs);
  vector<int> cx(cp), cy(cs);
  for (int i = 0; i < cp; ++i)if (flow.level[1 + i] == -1)cx[i] = 1;
  for (int i = 0; i < cs; ++i)
    if (flow.level[1 + cp + i] != -1)cy[i] = 1;
  for (int i = 0; i < n; ++i)
    if (cx[x[i]])
      vx[x[i]].push_back(i + 1);
    else
      vy[y[i]].push_back(i + 1);
  for (int i = 0; i < cp; ++i)
    if (!vx[i].empty())cout << vx[i].size() << ' ' << vx[i] << '\n';
  for (int i = 0; i < cs; ++i)
    if (!vy[i].empty())cout << vy[i].size() << ' ' << vy[i] << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

