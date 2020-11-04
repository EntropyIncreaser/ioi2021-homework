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

const int N = 200010;

int n;
int p[N], u[N], v[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("improvements.in", "r", stdin);
  freopen("improvements.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    int x;
    cin >> x;
    p[x] = i;
  }
  vector<int> cur;
  for (int i = 1; i <= n; ++i) {
    auto it = lower_bound(cur.begin(), cur.end(), p[i]);
    if (it == cur.end())cur.push_back(p[i]);
    else *it = p[i];
    u[i] = cur.size();
  }
  cur.clear();
  for (int i = n; i; --i) {
    auto it = lower_bound(cur.begin(), cur.end(), p[i]);
    if (it == cur.end())cur.push_back(p[i]);
    else *it = p[i];
    v[i] = cur.size();
  }
  int ans = 0;
  for (int i = 0; i <= n; ++i)
    ans = max(ans, u[i] + v[i + 1]);
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
