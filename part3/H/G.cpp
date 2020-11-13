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

const int N = 100010;

bool inf[2][N];
int win[2][N];
int d[2][N];

vector<int> g[N], gt[N];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("game.in", "r", stdin);
  freopen("game.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  while (m--) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    gt[v].push_back(u);
  }

  {
    for (int i = 1; i <= n; ++i) d[0][i] = g[i].size();
    for (int i = 0; i <= 1; ++i) fill_n(inf[i] + 1, n, true);
    queue<pair<int, int>> q;
    auto push = [&](int i, int j) {
      q.emplace(i, j);
      inf[i][j] = false;
    };
    for (int i = 1; i <= n; ++i)
      if (d[0][i] == 0) {
        push(0, i);
        push(1, i);
      }
    while (!q.empty()) {
      int f, x;
      tie(f, x) = q.front();
      q.pop();
      if (f == 0) {
        for (int y : gt[x])
          if (inf[1][y])
            push(1, y);
      } else
        for (int y : gt[x])
          if (--d[0][y] == 0)
            push(0, y);
    }
  }

  {
    queue<pair<int, int>> q;
    auto push = [&](int i, int j, int x) {
      if (win[i][j] != -1)
        return;
      q.emplace(i, j);
      win[i][j] = x;
    };
    for (int f = 0; f <= 1; ++f) for (int i = 1; i <= n; ++i) for (int j : g[i]) d[f][i] += !inf[!f][j];
    for (int i = 0; i <= 1; ++i) fill_n(win[i] + 1, n, -1);
    for (int f = 0; f <= 1; ++f)
      for (int i = 1; i <= n; ++i)
        if (d[f][i] == 0 && !inf[f][i])
          push(f, i, 0);
    while (!q.empty()) {
      int f, x;
      tie(f, x) = q.front();
      q.pop();
      if (win[f][x] == 0) {
        for (int y : gt[x])
          if (!inf[!f][y])
            push(!f, y, 1);
      } else
        for (int y : gt[x])
          if (!inf[!f][y] && --d[!f][y] == 0)
            push(!f, y, 0);
    }
  }
  for (int i = 0; i <= 1; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (!inf[i][j] && win[i][j] == -1)
        win[i][j] = !i;
      cout << "DLW"[win[i][j] + 1];
    }
    cout << '\n';
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
