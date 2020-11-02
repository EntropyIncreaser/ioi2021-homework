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

const int N = 55;

int n;
int v[N];
int cnt[3];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < n; ++j) {
      int t;
      cin >> t;
      v[j] |= t << i;
    }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 3; ++j)
      if (v[i] == 1 << j)
        cnt[j] = i + 1;
      else if (v[i] == 7 - (1 << j))
        cnt[j] = -(i + 1);
  }
  if (count(cnt, cnt + 3, 0) != 1) {
    cout << "-1\n";
    return 0;
  }
  vector<string> ans;
  auto param = [&](int x) { return (x < 0 ? "!x" : "x") + to_string(abs(x)); };
  auto iff = [&](int x, int y) {
    ans.push_back(param(x) + " -> " + param(y));
    ans.push_back(param(y) + " -> " + param(x));
  };
  for (int i = 0; i < n; ++i) {
    if (v[i] == 0) ans.push_back(param(i + 1) + " -> " + param(-(i + 1)));
    else if (v[i] == 7) ans.push_back(param(-(i + 1)) + " -> " + param(i + 1));
    else {
      for (int j = 0; j < 3; ++j)
        if (v[i] == 1 << j)
          iff(i + 1, cnt[j]);
        else if (v[i] == 7 - (1 << j))
          iff(-(i + 1), cnt[j]);
    }
  }
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < i; ++j)
      if (cnt[i] && cnt[j]) {
        ans.push_back(param(cnt[i]) + " -> " + param(-cnt[j]));
        break;
      }
  cout << ans.size() << '\n';
  for (const auto &str : ans) cout << str << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
