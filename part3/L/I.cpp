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

const int N = 100010;

int n;
int p[N], q[N];
pair<int, int> ans[N];
pair<int, int> rg[N];

void solve(int l, int r, const vector<tuple<int, int, int>> &query) {
  if (query.empty()) {
    return;
  }
  int mid = (l + r) >> 1;
  fill(rg + l, rg + r + 1, make_pair(-1, -1));
  rg[mid] = make_pair(mid, mid);
  int pl = mid, pr = mid, vl = p[mid], vr = p[mid], cl = mid, cr = mid;
  auto fix = [&]() {
    while (true) {
      if (cl < pl || cr > pr) {
        int x = p[cl < pl ? --pl : ++pr];
        while (vl > x || vr < x) {
          int j = q[vl > x ? --vl : ++vr];
          if (j < l || j > r) {
            return false;
          }
          cl = min(cl, j);
          cr = max(cr, j);
        }
      } else {
        return true;
      }
    }
    return true;
  };
  for (int i = mid - 1; i >= l; --i) {
    cl = min(cl, i);
    if (!fix()) {
      break;
    }
    rg[i] = make_pair(pl, pr);
  }
  pl = pr = cl = cr = mid;
  vl = vr = p[mid];
  for (int i = mid + 1; i <= r; ++i) {
    cr = max(cr, i);
    if (!fix()) {
      break;
    }
    rg[i] = make_pair(pl, pr);
  }
  vector<tuple<int, int, int>> ql, qr;
  for (const auto&[l, r, id] : query) {
    if (rg[l].first != -1 & rg[r].first != -1) {
      ans[id] = make_pair(min(rg[l].first, rg[r].first), max(rg[l].second, rg[r].second));
    }
    if (r < mid) {
      ql.emplace_back(l, r, id);
    } else if (l > mid) {
      qr.emplace_back(l, r, id);
    }
  }
  solve(l, mid - 1, ql);
  solve(mid + 1, r, qr);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> p[i];
  }
  for (int i = 1; i <= n; ++i) {
    q[p[i]] = i;
  }
  int m;
  cin >> m;
  vector<tuple<int, int, int>> query;
  for (int i = 1; i <= m; ++i) {
    int l, r;
    cin >> l >> r;
    query.emplace_back(l, r, i);
  }
  solve(1, n, query);
  for (int i = 1; i <= m; ++i) {
    cout << ans[i].first << ' ' << ans[i].second << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
