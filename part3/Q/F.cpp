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

template<class T>
void chkMax(T &x, const T &y) {
  if (x < y) x = y;
}

template<class T>
void chkMin(T &x, const T &y) {
  if (x > y) x = y;
}

ull gcd(ull a, ull b) { return b ? gcd(b, a % b) : a; }

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("fygon20.in", "r", stdin);
  freopen("fygon20.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  cin.ignore();
  vector<int> used(26);
  vector<vector<int>> g(26);
  for (int i = 0; i < n - 1; ++i) {
    string line;
    getline(cin, line);
    char it = line[i * 4 + 4], from = line[i * 4 + 15], to = line[i * 4 + 18];
    used[it - 'a'] = 1;
    if (from != '1') g[from - 'a'].push_back(it - 'a');
    if (to != 'n') g[it - 'a'].push_back(to - 'a');
  }

  vector<int> dfn(26, -1), low(26), label(26), ins(26);
  int t = 0, lab = 0;
  stack<int> stk;
  function<void(int)> dfs = [&](int u) {
    low[u] = dfn[u] = t++;
    ins[u] = 1;
    stk.push(u);
    for (int v : g[u]) {
      if (dfn[v] == -1)
        dfs(v);
      if (ins[v])
        chkMin(low[u], low[v]);
    }
    if (low[u] == dfn[u]) {
      while (stk.top() != u) {
        label[stk.top()] = lab;
        ins[stk.top()] = 0;
        stk.pop();
      }
      label[u] = lab;
      ins[u] = 0;
      stk.pop();
      ++lab;
    }
  };
  for (int i = 0; i != 26; ++i)
    if (used[i] && dfn[i] == -1)
      dfs(i);
  vector<int> g2(lab);
  for (int i = 0; i != 26; ++i)
    for (int j : g[i])
      if (label[i] != label[j])
        g2[label[i]] |= 1 << label[j];
  vector<ull> dp(1 << lab);
  dp[0] = 1;
  for (int s = 1; s != 1 << lab; ++s) {
    for (int i = 0; i != lab; ++i)
      if ((s >> i) & 1)
        if (!(g2[i] & s))
          dp[s] += dp[s ^ (1 << i)];
  }
  ull p = dp[(1 << lab) - 1], q = 1;
  for (int i = 1; i <= lab; ++i) q *= i;
  ull gc = gcd(p, q);
  p /= gc;
  q /= gc;
  cout << lab << ' ' << p << '/' << q << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
