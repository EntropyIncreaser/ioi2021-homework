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
#include <stack>
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

#ifndef ELEGIA
const int N = 500010;
#else
const int N = 100;
#endif

int n, m;
string s[N];
int nde[N][2];
int k[N], p[N], ans[N];
int t, root, pol;
int ch[N * 2][2], prt[N * 2];
int upc[N * 2], dwc[N * 2];

vector<int> g[N * 6];
vector<int> col[N * 2];

void GG() {
  cout << "NO\n";
  exit(0);
}

int ins(const string &str) {
  int o = root;
  for (char c:str) {
    c -= '0';
    if (!ch[o][c]) {
      ch[o][c] = ++t;
      prt[t] = o;
    }
    o = ch[o][c];
  }
  return o;
}

int scc[N * 6], low[N * 6], dfn[N * 6];

void dfs(int u) {
  static int t;
  static bool ins[N * 6];
  static stack<int> stk;
  if (dfn[u])
    return;
  low[u] = dfn[u] = ++t;
  stk.push(u);
  ins[u] = true;
  for (int v:g[u]) {
    dfs(v);
    if (ins[v])
      low[u] = min(low[u], low[v]);
  }
  if (low[u] == dfn[u]) {
    int v;
    do {
      v = stk.top();
      stk.pop();
      ins[v] = false;
      scc[v] = u;
      if (v < m * 2) {
        if (ans[v >> 1] == -1)
          ans[v >> 1] = v & 1;
        if (scc[v ^ 1] == u) {
          GG();
        }
      }
    } while (v != u);
  }
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("binary.in", "r", stdin);
  freopen("binary.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < n; ++i)cin >> s[i];
  root = ++t;
  for (int i = 0; i < n; ++i) {
    auto it = s[i].find('?');
    if (it != s[i].npos) {
      k[m] = i;
      p[m] = it;
      ++m;
      s[i][it] = '0';
      nde[i][0] = ins(s[i]);
      s[i][it] = '1';
      nde[i][1] = ins(s[i]);
      s[i][it] = '?';
    } else {
      nde[i][0] = ins(s[i]);
      nde[i][1] = -1;
      ++upc[nde[i][0]];
      ++dwc[nde[i][0]];
    }
  }
  for (int i = t; i; --i)
    upc[i] += upc[ch[i][0]] + upc[ch[i][1]];
  for (int i = 1; i <= t; ++i)
    for (int j = 0; j <= 1; ++j)
      if (ch[i][j])
        dwc[ch[i][j]] += dwc[i];
  for (int i = 0; i < n; ++i)
    if (nde[i][1] == -1 && upc[nde[i][0]] > 1)GG();
  for (int i = 1; i <= t; ++i) {
    for (int j = 0; j <= 1; ++j) {
      int u = i - 1, v = ch[i][j] - 1;
      if (v == -1)continue;
      g[m * 2 + u * 2].push_back(m * 2 + v * 2);
      g[m * 2 + v * 2 + 1].push_back(m * 2 + u * 2 + 1);
    }
  }
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j <= 1; ++j) {
      int nd = nde[k[i]][j];
      col[nd].push_back(i << 1 | j);
      if (dwc[nd] || upc[nd])
        g[i << 1 | j].push_back(i << 1 | !j);
      g[m * 2 + (nd - 1) * 2].push_back(i << 1 | !j);
      g[m * 2 + (nd - 1) * 2 + 1].push_back(i << 1 | !j);
      for (int x = 0; x <= 1; ++x)
        if (ch[nd][x])
          g[i << 1 | j].push_back(m * 2 + (ch[nd][x] - 1) * 2);
      if (prt[nd])g[i << 1 | j].push_back(m * 2 + (prt[nd] - 1) * 2 + 1);
    }
  }
  pol = m * 2 + t * 2;
  for (int i = 1; i <= t; ++i)
    if (col[i].size() > 1) {
      vector<int> pre(col[i].size() - 1), suf(col[i].size() - 1);
      for (int &x:pre)x = pol++;
      for (int &x:suf)x = pol++;
      for (int j = 0; j < pre.size(); ++j) {
        g[pre[j]].push_back(col[i][j] ^ 1);
        g[suf[j]].push_back(col[i][j + 1] ^ 1);
        g[col[i][j + 1]].push_back(pre[j]);
        g[col[i][j]].push_back(suf[j]);
      }
      for (int j = 1; j < pre.size(); ++j) {
        g[pre[j]].push_back(pre[j - 1]);
        g[suf[j - 1]].push_back(suf[j]);
      }
    }
  memset(scc, -1, sizeof(scc));
  memset(ans, -1, sizeof(ans));
  for (int i = 0; i < m * 2; ++i)
    dfs(i);
  for (int i = 0; i < m; ++i)
    s[k[i]][p[i]] = '0' + ans[i];
  cout << "YES\n";
  for (int i = 0; i < n; ++i)
    cout << s[i] << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
