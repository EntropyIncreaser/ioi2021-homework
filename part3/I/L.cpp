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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
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

const int N = 100010;

int n;
ll v;
int w[N];
ll sum[N];
int l[N], r[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("landscape.in", "r", stdin);
  freopen("landscape.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> v;
  for (int i = 1; i <= n; ++i) {
    cin >> w[i];
  }
  for (int i = 1; i <= n; ++i) {
    sum[i] = sum[i - 1] + w[i];
  }

  auto check = [&](int ht) {
    vector<int> stk, pos;
    for (int i = n; i; --i) {
      int push = w[i] + i;
      while (!stk.empty() && stk.back() <= push) {
        stk.pop_back();
        pos.pop_back();
      }
      stk.push_back(push);
      pos.push_back(i);
      int goal = ht + i;
      int id = upper_bound(stk.begin(), stk.end(), goal, greater<int>()) - stk.begin();
      if (id == 0) {
        r[i] = -1;
      } else {
        r[i] = pos[id - 1] - 1;
      }
    }
    stk.clear();
    pos.clear();
    for (int i = 1; i <= n; ++i) {
      int push = w[i] - i;
      while (!stk.empty() && stk.back() <= push) {
        stk.pop_back();
        pos.pop_back();
      }
      stk.push_back(push);
      pos.push_back(i);
      int goal = ht - i;
      int id = upper_bound(stk.begin(), stk.end(), goal, greater<int>()) - stk.begin();
      if (id == 0) {
        l[i] = -1;
      } else {
        l[i] = pos[id - 1] + 1;
      }
    }
    for (int i = 1; i <= n; ++i) {
      if (l[i] != -1 && r[i] != -1) {
        ll tot = -(sum[r[i]] - sum[l[i] - 1]);
        tot += (i - l[i] + 1) * (ll)(ht + ht - (i - l[i])) / 2;
        tot += (r[i] - i + 1) * (ll)(ht + ht - (r[i] - i)) / 2;
        tot -= ht;
        if (tot <= v) {
          return true;
        }
      }
    }
    return false;
  };

  int low = *max_element(w + 1, w + n + 1), high = 200000;
  while (low < high) {
    int mid = (low + high + 1) / 2;
    if (check(mid)) {
      low = mid;
    } else {
      high = mid - 1;
    }
  }
  cout << low << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
