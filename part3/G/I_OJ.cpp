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

#include "sort.h"

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void work(int n) {
  int cnt = 0;
  vector<int> e(n / 2), o((n + 1) / 2);
  auto _ask = [&](int x, int y) {
    return ask(x + 1, y + 1);
  };
  function<void(vector<int>, vector<int>, bool)> solve = [&](vector<int> ie, vector<int> io,
                                                             bool side) {
    if (side == 0) {
      while (!ie.empty()) {
        int x = ie.front();
        auto it = partition(io.begin(), io.end(), [&](int y) { return _ask(x, y); });
        solve(ie, vector<int>(io.begin(), it), 1);
        e[x] = ++cnt;
        io.erase(io.begin(), it);
        ie.erase(partition(ie.begin(), ie.end(), [&](int x) { return e[x] == 0; }), ie.end());
      }
    } else {
      while (!io.empty()) {
        int y = io.front();
        auto it = partition(ie.begin(), ie.end(), [&](int x) { return !_ask(x, y); });
        solve(vector<int>(ie.begin(), it), io, 0);
        o[y] = ++cnt;
        ie.erase(ie.begin(), it);
        io.erase(partition(io.begin(), io.end(), [&](int y) { return o[y] == 0; }), io.end());
      }
    }
  };
  vector<int> ie(n / 2), io((n + 1) / 2);
  iota(ie.begin(), ie.end(), 0);
  iota(io.begin(), io.end(), 0);
  shuffle(ie.begin(), ie.end(), rng);
  shuffle(io.begin(), io.end(), rng);
  solve(ie, io, n & 1);
  answer(e, o);
}
