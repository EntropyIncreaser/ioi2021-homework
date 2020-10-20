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
#include <bits/stdc++.h>

#define LOG(FMT...) fprintf(stderr, FMT)

#define fir first
#define sec second

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
}

ostream &operator<<(ostream &os, const pair<char, int> &unit) {
  return os << unit.first << "^" << unit.second;
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

const int N = 4010;

int n;
string goal;
int last[N];
string s[N];

void gg() {
  cout << "impossible\n";
  exit(0);
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> goal;
  for (int i = 1; i <= n; ++i) cin >> s[i];
  sort(s + 1, s + n + 1, [&](const string &a, const string &b) {
    return a.length() < b.length();
  });
  auto test = [&](const string &a, const string &b) {
    if (a.empty())return true;
    int i = 0;
    for (char c : b) {
      if (a[i] == c)
        if (++i == a.length())
          return true;
    }
    return false;
  };
  int x = 0, y = 0, head = -1, tail = -1;
  for (int i = 1; i <= n; ++i) {
    if (head == -1) {
      if (test(s[x], s[i])) {
        if (test(s[y], s[i])) {
          head = tail = i;
        } else {
          last[i] = x;
          x = i;
        }
      } else {
        if (test(s[y], s[i])) {
          last[i] = y;
          y = i;
        } else gg();
      }
    } else if (test(s[tail], s[i])) {
      last[i] = tail;
      tail = i;
    } else {
      if (!test(s[x], s[i]))swap(x, y);
      if (!test(s[x], s[i]))gg();
      last[i] = x;
      last[head] = y;
      x = i;
      y = tail;
      head = -1;
    }
  }
  if (head != -1) {
    last[head] = x;
    x = tail;
  }
  if (!test(s[x], goal) || !test(s[y], goal))
    gg();
  auto trace = [&](int x) {
    vector<string> vec;
    while (x) {
      vec.push_back(s[x]);
      x = last[x];
    }
    reverse(vec.begin(), vec.end());
    return vec;
  };
  auto a1 = trace(x), a2 = trace(y);
  cout << a1.size() << ' ' << a2.size() << '\n';
  for (string str:a1)cout << str << '\n';
  for (string str:a2)cout << str << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

