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

using Inter = vector<ull>;

const int X = 10;

const Inter ONE(X, 1);

Inter per(Inter a) {
  for (int i = 1; i < X; ++i) a[i] = a[i - 1] + a[i];
  a.insert(a.begin(), 0);
  a.pop_back();
  return a;
}
Inter operator+(const Inter &lhs, const Inter &rhs) {
  Inter ret(X, 0);
  for (int i = 0; i < X; ++i) ret[i] = lhs[i] + rhs[i];
  return ret;
}
Inter operator*(const Inter &lhs, const Inter &rhs) {
  Inter ret(X, 0);
  for (int i = 0; i < X; ++i) ret[i] = lhs[i] * rhs[i];
  return ret;
}

ull C[X][X];

int prt[27];
vector<int> ch[27];

Inter dfs(int x) {
  Inter ret = ONE;
  if (x <= 0) {
    for (int i = 1; i <= 26; ++i)
      if (prt[i] == x)ret = ret * per(dfs(i));
  } else
    for (int i : ch[x])
      ret = ret * per(dfs(i));
  return ret;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("fygon.in", "r", stdin);
  freopen("fygon.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  stack<int> stk;
  Inter ans(X, 0);
  fill(prt + 1, prt + 27, -999);
  while (getline(cin, line)) {
    int indent = 0;
    while (line[indent * 4] == ' ') ++indent;
    while (stk.size() > indent) {
      int x = stk.top();
      stk.pop();
      if (prt[x] > 0)ch[prt[x]].pop_back();
      prt[x] = -999;
    }
    if (line[indent * 4] == 'f') {
      char it = line[indent * 4 + 4], to = line[indent * 4 + 15];
      int x = it - 'a' + 1, y = to - 'a' + 1;
      if (isdigit(to)) y = -(to - '0');
      if (to == 'n') y = 0;
      stk.push(x);
      prt[x] = y;
      if (y > 0) ch[y].push_back(x);
    } else {
      Inter ad = dfs(0);
      for (int i = 1; i <= 9; ++i) ad = ad * Inter(X, dfs(-i)[i]);
      ans = ans + ad;
    }
  }

  for (int i = 0; i < X; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j)
      C[i][j] = -C[i - 1][j - 1] + C[i - 1][j];
  }
  for (int i = X - 1; i >= 0; --i)
    for (int j = 0; j < i; ++j)
      ans[i] = ans[i] + ans[j] * C[i][i - j];
  int k = 9;
  while (!ans[k])--k;
  ull fac = 1;
  for (int i = 0; i <= k; ++i) {
    if (i > 0)fac *= i;
    cout << ans[i] << " / " << fac;
    if (i < k) {
      cout << " + ";
      if (i == 0) cout << 'n';
      else cout << "(n - " << i << ')';
      cout << " * (";
    }
  }
  while (k--) cout << ')';
  cout << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

