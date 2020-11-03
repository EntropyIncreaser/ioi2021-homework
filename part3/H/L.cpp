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

const ull INF = ull(1e18) + 5;

const int TH = 2600;

int n;
bool vis[TH];
int pr[TH], len[TH];
ull cnt[TH][TH], sum[TH][TH];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("list.in", "r", stdin);
  freopen("list.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 2; i < TH; ++i) {
    if (!vis[i]) {
      pr[n++] = i;
      for (int j = i; j < TH; j += i) vis[j] = true;
    }
  }
  cnt[n][0] = 1;
  sum[n][0] = 2;
  for (int i = n - 1; i >= 0; --i) {
    len[i] = to_string(pr[i]).length() + 2;
    memcpy(cnt[i], cnt[i + 1], sizeof(cnt[i]));
    memcpy(sum[i], sum[i + 1], sizeof(sum[i]));
    for (int j = pr[i]; j < TH; ++j) {
      cnt[i][j] = min(INF, cnt[i][j] + cnt[i + 1][j - pr[i]]);
      sum[i][j] = min(INF, sum[i][j] + sum[i + 1][j - pr[i]] + cnt[i + 1][j - pr[i]] * len[i]);
    }
  }

  ull a, b;
  cin >> a >> b;

  string cur, ans;
  int progress = 0;
  ull f = 0, g = 0;
  function<void(int, int)> dfs = [&](int i, int j) {
    if (progress == 0) {
      if (f + sum[i][j] + cur.length() * cnt[i][j] < a) {
        f += sum[i][j] + cur.length() * cnt[i][j];
        return;
      }
    }
    if (i == n) {
      progress = 1;
      g += cur.length() + 2;
      cur.erase(cur.end() - 2, cur.end());
      ans += "[" + cur + "], ";
      cur += ", ";
      if (f + g > b) progress = 2;
      return;
    }
    if (j >= pr[i] && cnt[i + 1][j - pr[i]]) {
      int l = cur.length();
      cur += to_string(pr[i]) + ", ";
      dfs(i + 1, j - pr[i]);
      cur.erase(cur.begin() + l, cur.end());
    }
    if (progress == 2) return;
    if (cnt[i + 1][j]) dfs(i + 1, j);
  };

  for (int i = 1; i < TH; ++i) {
    dfs(0, i);
    if (progress == 2) break;
  }
  ans.erase(ans.begin(), ans.begin() + (a - f - 1));
  ans.erase(ans.begin() + (b - a + 1), ans.end());
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
