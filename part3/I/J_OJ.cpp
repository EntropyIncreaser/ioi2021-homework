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

#include "jump.h"

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

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

void solve(int n) {
  auto ask = [&](const string &s) {
    vector<int> vec(n);
    for (int i = 0; i < n; ++i) vec[i] = s[i] == '1';
    int v = query(vec);
    if (v == n) {
      exit(0);
    }
    return v == n / 2;
  };

  string s(n, ' ');
  while (true) {
    for (int i = 0; i < n; ++i) {
      s[i] = '0' + (rng() & 1);
    }
    if (ask(s)) {
      break;
    }
  }
  auto flip = [&](char c) {
    return ((c - '0') ^ 1) + '0';
  };
  vector<int> color(n);
  s[0] = flip(s[0]);
  for (int i = 1; i < n; ++i) {
    s[i] = flip(s[i]);
    color[i] = ask(s);
    s[i] = flip(s[i]);
  }
  s[0] = flip(s[0]);
  for (int i = 0; i < n; ++i) {
    if (color[i]) {
      s[i] = flip(s[i]);
    }
  }
  ask(s);
  for (int i = 0; i < n; ++i) {
    s[i] = flip(s[i]);
  }
  ask(s);
}
