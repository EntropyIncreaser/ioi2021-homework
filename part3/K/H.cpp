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
#include <unordered_map>
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

using uint = unsigned int;

const int N = 100010;

int n;
uint a[N], xsum[N];
vector<uint> query[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("hack.in", "r", stdin);
  freopen("hack.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    xsum[i] = xsum[i - 1] ^ a[i];
  }
  vector<pair<uint, int>> segments;
  for (int i = 1; i <= n; ++i) {
    for (auto &pr : segments) {
      pr.first &= a[i];
    }
    segments.emplace_back(a[i], i);
    vector<pair<uint, int>> tmp;
    for (int j = 0; j != segments.size(); ++j) {
      if (!j || segments[j].first != segments[j - 1].first) {
        tmp.push_back(segments[j]);
      }
    }
    swap(tmp, segments);
    for (int j = 0; j != segments.size(); ++j) {
      int r = (j + 1) == segments.size() ? i : segments[j + 1].second - 1;
      query[segments[j].second - 1].push_back(xsum[i] ^ segments[j].first ^ 1u << 31);
      query[r].push_back(xsum[i] ^ segments[j].first);
    }
  }
  unordered_map<uint, int> mp;
  ll ans = 0;
  for (int i = 1; i <= n; ++i) {
    ++mp[xsum[i - 1]];
    for (auto x : query[i]) {
      if ((x >> 31) & 1) {
        ans -= mp[x ^ 1u << 31];
      } else {
        ans += mp[x];
      }
    }
  }
  cout << ans << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
