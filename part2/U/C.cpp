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

const int Mod = 998244353, Base = 114514;

const int N = 2010;

int n, m;
char s[N], t[N];
int sh[N], th[N], power[N];

bool cover[N];
int nxt[N];

void buildHash(char *str, int *hash, int len) {
  for (int i = 0; i < len; ++i)
    hash[i + 1] = (hash[i] * (ull) Base + str[i]) % Mod;
  power[0] = 1;
  for (int i = 1; i <= len; ++i) power[i] = power[i - 1] * (ull) Base % Mod;
}

int getHash(int *hash, int l, int r) {
  return (hash[r] + (Mod - power[r - l]) * (ull) hash[l]) % Mod;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("curiosity.in", "r", stdin);
  freopen("curiosity.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin.getline(s, 2010);
  cin.getline(t, 2010);
  n = strlen(s);
  m = strlen(t);
  buildHash(s, sh, n);
  buildHash(t, th, m);
  tuple<int, int, int, int> ans(numeric_limits<int>::max(), 0, 0, 0);
  for (int len = 1; len <= n; ++len) {
    memset(cover, 0, sizeof(cover));
    memset(nxt, -1, sizeof(nxt));
    unordered_map<int, int> exist, last;
    for (int i = n - len; i >= 0; --i) {
      if (i + len * 2 <= n) {
        int val = getHash(sh, i + len, i + len * 2);
        exist[val] = i + len;
      }
      int val = getHash(sh, i, i + len);
      if (exist.count(val)) nxt[i] = exist[val];
      if (last.count(val)) cover[last[val]] = true;
      last[val] = i;
    }
    for (int i = 0; i <= n - len; ++i) {
      if (cover[i]) continue;
      int cnt = 0;
      int x = i;
      while (x != -1) {
        x = nxt[x];
        ++cnt;
      }
      int rest = n - cnt * len;
      if (m < rest || (m - rest) % cnt) continue;
      int lem = (m - rest) / cnt;
      int val = getHash(th, i, i + lem);
      int cur = 0;
      int p = 0;
      x = i;
      while (x != -1) {
        cur = (cur * (ull) power[x - p] + getHash(sh, p, x)) % Mod;
        cur = (cur * (ull) power[lem] + val) % Mod;
        p = x + len;
        x = nxt[x];
      }
      cur = (cur * (ull) power[n - p] + getHash(sh, p, n)) % Mod;
      if (cur == th[m])
        ans = min(ans, make_tuple(len + lem, i, len, lem));
    }
  }
  int i, len, lem;
  tie(ignore, i, len, lem) = ans;
  cout << "s/" << string(s + i, s + i + len) << "/" << string(t + i, t + i + lem) << "/g\n";

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

