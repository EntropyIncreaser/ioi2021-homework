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

typedef vector<int> vi;

const int B = 500;

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

  int goal;
  cin >> goal;

  int rep = 0;
  vector<vi> pow10;
  pow10.emplace_back(B, 0);
  pow10[0][0] = 1;
  auto getPow10 = [&](int n) {
    while (n >= pow10.size()) {
      vi t(B);
      for (int i = B - 1; i >= 0; --i) {
        if (i) {
          t[i] = pow10.back()[i - 1];
        }
        if (i >= 3) {
          t[i] += pow10.back()[i - 3];
        }
      }
      int carry = 0;
      for (int i = 0; i < B; ++i) {
        t[i] += carry;
        carry = t[i] / 2;
        t[i] -= carry * 2;
      }
      pow10.push_back(t);
    }
    return pow10[n];
  };

  auto add = [&](vi a, const vi& b) {
    int carry = 0;
    for (int i = 0; i < B; ++i) {
      a[i] += b[i] + carry;
      carry = a[i] / 2;
      a[i] -= carry * 2;
    }
    return a;
  };

  vector<pair<vi, vi>> vec;
  vec.emplace_back(vi(1, 0), vi(B, 0));
  vec.emplace_back(vi(1, 1), getPow10(0));
  int k = 0;
  while (true) {
    vector<pair<vi, vi>> v0, v1;
    for (auto pr : vec) {
      if (pr.first[k] == 1 && ++rep == goal) {
        string ans;
        for (int i = k; i >= 0; --i) {
          ans.push_back('0' + pr.first[i]);
        }
        cout << ans << '\n';
        exit(0);
      }
      if (pr.second[k + 1] == 0) {
        pr.first.push_back(0);
        v0.emplace_back(pr.first, pr.second);
        pr.first.back() = 1;
        v1.emplace_back(pr.first, add(pr.second, getPow10(k + 1)));
      }
    }
    swap(v0, vec);
    vec.insert(vec.end(), v1.begin(), v1.end());
    ++k;
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
