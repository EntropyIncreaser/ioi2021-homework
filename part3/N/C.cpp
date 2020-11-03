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

#ifdef ELEGIA
const int X = 100, N = 210;
#else
const int X = 1000000, N = 200010;
#endif

namespace SEG {

int tag[1 << 21];

void pd(int o) {
  if (tag[o] != -1) {
    tag[o << 1] = tag[o << 1 | 1] = tag[o];
    tag[o] = -1;
  }
}

void change(int o, int l, int r, int cl, int cr, int x) {
  if (l == cl && r == cr) {
    tag[o] = x;
    return;
  }
  pd(o);
  int mid = (l + r) >> 1;
  if (cr <= mid) change(o << 1, l, mid, cl, cr, x);
  else if (cl > mid) change(o << 1 | 1, mid + 1, r, cl, cr, x);
  else {
    change(o << 1, l, mid, cl, mid, x);
    change(o << 1 | 1, mid + 1, r, mid + 1, cr, x);
  }
}

int qry(int o, int l, int r, int k) {
  if (tag[o] != -1)return tag[o];
  if (l == r)return 0;
  int mid = (l + r) >> 1;
  return k <= mid ? qry(o << 1, l, mid, k) : qry(o << 1 | 1, mid + 1, r, k);
}

}

int ans[N];
int fw[X + 10];
int r1[N], r2[N], c1[N], c2[N];
int fr[N], fc[N];
int nr[N], nc[N];
vector<int> sub[N];
vector<tuple<int, int, int>> sing[N];
int val[N * 2], prt[N * 2];

void add(int k, int x) {
  for (; k <= X; k += k & -k)fw[k] += x;
}

int qry(int k) {
  int ret = 0;
  for (; k; k &= k - 1)ret += fw[k];
  return ret;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int f, m, n;
  cin >> f;
  auto readpos = [&]() {
    int x;
    cin >> x;
    return X + 1 - x;
  };
  for (int i = 1; i <= f; ++i) {
    r2[i] = readpos();
    c2[i] = readpos();
    r1[i] = readpos();
    c1[i] = readpos();
  }
  cin >> m;
  for (int i = 1; i <= m; ++i) {
    fr[i] = readpos();
    fc[i] = readpos();
  }
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    nr[i] = readpos();
    nc[i] = readpos();
  }

  {
    static vector<pair<int, int>> single[X + 10];
    static vector<int> range[X + 10];
    map<int, int> mp;
    mp.insert(make_pair(X + 10, 0));
    for (int i = 1; i <= f; ++i) {
      range[r1[i]].push_back(i);
      range[r2[i] + 1].push_back(-i);
    }
    for (int i = 1; i <= m; ++i) single[fr[i]].emplace_back(fc[i], 0);
    for (int i = 1; i <= n; ++i) single[nr[i]].emplace_back(nc[i], i);
    for (int i = 1; i <= X; ++i) {
      for (int id : range[i])
        if (id > 0) {
          auto it = mp.insert(make_pair(c2[id], id)).first;
          sub[(++it)->second].push_back(id);
          mp.insert(make_pair(c1[id] - 1, it->second));
        } else {
          mp.erase(c1[-id] - 1);
          mp.erase(c2[-id]);
        }
      for (const auto &pr : single[i])
        sing[mp.lower_bound(pr.first)->second].emplace_back(i, pr.first, pr.second);
    }
  }
  r1[0] = c1[0] = 1;
  for (int i = 0; i <= f; ++i) {
    vector<tuple<int, int, int>> moments;
    vector<pair<int, int>> ranges;

    sort(sing[i].begin(), sing[i].end());
    for (const auto&[x, y, z] : sing[i])
      if (z) ans[z] += qry(y);
      else add(y, 1);
    for (const auto&[x, y, z] : sing[i])
      if (!z) add(y, -1);

    for (int id : sub[i])
      ranges.emplace_back(r2[id], id);
    for (const auto&[x, y, z] : sing[i])
      if (z) ranges.emplace_back(x, -z);
      else moments.emplace_back(x, y, 0);
    sort(ranges.begin(), ranges.end());
    SEG::change(1, 0, X, 0, X, 0);
    for (auto[x, y] : ranges)
      if (y > 0) {
        val[y] = 0;
        int p = prt[y] = SEG::qry(1, 0, X, c1[y] - 1);
        moments.emplace_back(r2[p], c1[y] - 1, y);
        moments.emplace_back(r2[p], c1[p] - 1, y);
        SEG::change(1, 0, X, c1[y], c2[y], y);
      } else {
        y = -y;
        val[f + y] = 0;
        int p = prt[f + y] = SEG::qry(1, 0, X, nc[y]);
        moments.emplace_back(r2[p], nc[y], f + y);
        moments.emplace_back(r2[p], c1[p] - 1, f + y);
      }
    sort(moments.begin(), moments.end());
    for (auto[x, y, z] : moments)
      if (z == 0) add(y, 1);
      else {
        int v = qry(y);
        if (z <= f) {
          if (y == c1[z] - 1) val[z] += v;
          else val[z] -= v;
        } else {
          z -= f;
          if (y == nc[z]) val[f + z] += v;
          else val[f + z] -= v;
        }
      }
    for (auto[x, y, z] : moments)
      if (z == 0) add(y, -1);
    for (auto[x, y] : ranges)
      if (y > 0) {
        int p = prt[y];
        val[y] += val[p];
      } else {
        y = -y;
        int p = prt[f + y];
        val[f + y] += val[p];
        ans[y] -= val[f + y];
      }

    ranges.clear();
    moments.clear();

    for (int id : sub[i])
      ranges.emplace_back(c2[id], id);
    for (const auto&[x, y, z] : sing[i])
      if (z) ranges.emplace_back(y, -z);
      else moments.emplace_back(y, x, 0);
    sort(ranges.begin(), ranges.end());
    SEG::change(1, 0, X, 0, X, 0);
    for (auto[x, y] : ranges)
      if (y > 0) {
        val[y] = 0;
        int p = prt[y] = SEG::qry(1, 0, X, r1[y] - 1);
        moments.emplace_back(c2[p], r1[y] - 1, y);
        moments.emplace_back(c2[p], r1[p] - 1, y);
        SEG::change(1, 0, X, r1[y], r2[y], y);
      } else {
        y = -y;
        val[f + y] = 0;
        int p = prt[f + y] = SEG::qry(1, 0, X, nr[y]);
        moments.emplace_back(c2[p], nr[y], f + y);
        moments.emplace_back(c2[p], r1[p] - 1, f + y);
      }
    sort(moments.begin(), moments.end());
    for (auto[x, y, z] : moments)
      if (z == 0) add(y, 1);
      else {
        int v = qry(y);
        if (z <= f) {
          if (y == r1[z] - 1) val[z] += v;
          else val[z] -= v;
        } else {
          z -= f;
          if (y == nr[z]) val[f + z] += v;
          else val[f + z] -= v;
        }
      }
    for (auto[x, y, z] : moments)
      if (z == 0) add(y, -1);
    for (auto[x, y] : ranges)
      if (y > 0) {
        int p = prt[y];
        val[y] += val[p];
      } else {
        y = -y;
        int p = prt[f + y];
        val[f + y] += val[p];
        ans[y] -= val[f + y];
      }
  }
  for (int i = 1; i <= n; ++i) cout << ans[i] << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
