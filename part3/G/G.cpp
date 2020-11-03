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

struct Select {
  vector<ll> disc;
  vector<pair<ll, int>> ins;
  ll lazy;

  void insert(ll k, int v) {
    ins.emplace_back(k - lazy, v);
    disc.push_back(k - lazy);
  }

  void query(ll x) {
    ins.emplace_back(x - lazy, 0);
  }

  void build() {
    sort(disc.begin(), disc.end());
    disc.erase(unique(disc.begin(), disc.end()), disc.end());
    for (auto &pr : ins)
      if (pr.second)
        pr.first = lower_bound(disc.begin(), disc.end(), pr.first) - disc.begin() + 1;
  }

  ll eval(ll x) {
    int n = disc.size();
    vector<int> fw(n + 1);
    auto add = [&](int k, int x) {
      for (; k <= n; k += k & -k)
        fw[k] += x;
    };
    auto qry = [&](int k) {
      int ret = 0;
      for (; k; k &= k - 1)
        ret += fw[k];
      return ret;
    };
    ll ret = 0;
    for (const auto &pr:ins)
      if (pr.second) add(pr.first, pr.second);
      else ret += qry(upper_bound(disc.begin(), disc.end(), x + pr.first) - disc.begin());
    return ret;
  }
};

vector<int> a, b, c;

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, r;
  ll k;
  cin >> n >> r >> k;
  a.resize(n);
  b.resize(n);
  c.resize(n);
  cin >> a >> b >> c;
  ll sum = 0;
  for (int i = 0; i < n; ++i) {
    c[i] -= b[i];
    b[i] -= a[i];
    sum += a[i];
  }
  Select cov, inter;

  ll s0 = 0, s1 = 0, s2 = 0;
  for (int i = 0; i < n; ++i) {
    s1 += b[i];
    if (i >= r) {
      s1 -= b[i - r];
      s0 += b[i - r];
    }
    if (i >= r * 2)s0 -= b[i - r * 2];
    if (i >= r * 2 - 1) {
      cov.insert(s0, 1);
      cov.query(-s1);
    }
    s2 += c[i];
    if (i >= r)s2 -= c[i - r];
    if (i >= r * 2 - 1)inter.insert(s0, -1);
    if (i >= r)inter.query(-s1);
    if (i >= r - 1) {
      inter.insert(s2, 1);
      inter.lazy += b[i - r + 1] - c[i - r + 1];
    }
  }
  cov.build();
  inter.build();

  ll low = 0, high = accumulate(b.begin(), b.end(), 0LL) + accumulate(c.begin(), c.end(), 0LL);
  while (low < high) {
    ll mid = (low + high) / 2;
    if (cov.eval(mid) + inter.eval(mid) < k)
      low = mid + 1;
    else
      high = mid;
  }
  cout << (low + sum) << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
