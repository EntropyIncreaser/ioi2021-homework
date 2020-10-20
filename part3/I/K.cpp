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

const int N = 100100, M = 45;

set<int> in[N], out[N];
list<int> compress[N];
int sz;
int vertex[M], label[N];
int g[M][M];
int must[M];

void gg() {
  cout << "There is no route, Karl!\n";
  exit(0);
}

vector<int> ans;

bool dfs(int u, ll s) {
  if (s == (1LL << sz) - 1) {
    if (g[sz][u] != -1 && (must[sz] == -1 || must[sz] == u)) {
      for (int v : compress[g[sz][u]]) {
        ans.push_back(v);
      }
      return true;
    }
    return false;
  }
  for (int v = 0; v < sz; ++v) {
    if (!((s >> v) & 1) && g[v][u] != -1 && (must[v] == -1 || must[v] == u)) {
      if (dfs(v, s | (1LL << v))) {
        ans.pop_back();
        for (int x : compress[g[v][u]]) {
          ans.push_back(x);
        }
        return true;
      }
    }
  }
  return false;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("king.in", "r", stdin);
  freopen("king.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= m; ++i) {
    int u, v;
    cin >> u >> v;
    in[v].insert(i);
    out[u].insert(i);
    compress[i].push_back(u);
    compress[i].push_back(v);
  }
  for (int i = 1; i <= n; ++i) {
    if (in[i].empty() || out[i].empty()) {
      gg();
    }
  }
  for (int i = 2; i <= n; ++i) {
    if (in[i].size() == 1 && out[i].size() == 1) {
      int x = *in[i].begin(), y = *out[i].begin();
      int l = compress[x].front(),
          r = compress[y].back();
      if (l == i || r == i) {
        gg();
      }
      compress[x].pop_back();
      compress[x].splice(compress[x].end(), compress[y]);
      in[r].erase(y);
      in[r].insert(x);
    }
  }
  memset(label, -1, sizeof(label));
  for (int i = 2; i <= n; ++i) {
    if (in[i].size() > 1 || out[i].size() > 1) {
      label[i] = sz;
      vertex[sz] = i;
      ++sz;
    }
  }
  label[1] = sz;
  vertex[sz] = 1;
  memset(g, -1, sizeof(g));
  memset(must, -1, sizeof(must));
  for (int i = 1; i <= n; ++i) {
    if (i == 1 || in[i].size() > 1 || out[i].size() > 1) {
      for (int j : out[i]) {
        int y = compress[j].back();
        if (g[label[i]][label[y]] == -1) {
          g[label[i]][label[y]] = j;
        }
        if (compress[j].size() > 2) {
          if (must[label[i]] != -1) {
            gg();
          }
          must[label[i]] = label[y];
          g[label[i]][label[y]] = j;
        }
      }
    }
  }
  if (sz == 0) {
    for (int x : compress[*out[1].begin()]) {
      ans.push_back(x);
    }
  } else {
    if (!dfs(sz, 0)) {
      gg();
    }
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

