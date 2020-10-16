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

const int N = 110;

int a[N][N];
bool sgn[N / 2][N / 2];
int S4[4];
int vis[N * N];
bool r[N], c[N];

bool SGN() {
  static bool vis[4];
  memset(vis, 0, sizeof(vis));
  bool ret = false;
  for (int i = 0; i < 4; ++i) {
    if (vis[i]) {
      continue;
    }
    ret = !ret;
    int x = i;
    while (!vis[x]) {
      vis[x] = true;
      x = S4[x];
    }
  }
  return ret;
}

constexpr int pid(int s0, int s1, int s2, int s3) {
  return (((s0 * 4) + s1) * 4 + s2) * 4 + s3;
}

#define GG do { cout << "IMPOSSIBLE\n"; return; } while (false)

void solve() {
  memset(vis, -1, sizeof(vis));
  int W, H;
  cin >> W >> H;
  for (int i = 0; i < H; ++i) {
    for (int j = 0; j < W; ++j) {
      cin >> a[i][j];
      --a[i][j];
    }
  }
  auto getID = [&](int x, int y) {
    return x * W + y;
  };
  vector<pair<char, int>> ans;
  bool real = true;
  auto row = [&](int i) {
    if (real) {
      reverse(a[i], a[i] + W);
    }
    ans.emplace_back('R', i + 1);
  };
  auto col = [&](int j) {
    if (real) {
      for (int i = 0; i < H - 1 - i; ++i) {
        swap(a[i][j], a[H - 1 - i][j]);
      }
    }
    ans.emplace_back('C', j + 1);
  };
  if (H % 2) {
    static int goal[N];
    iota(goal, goal + W, H / 2 * W);
    if (!equal(goal, goal + W, a[H / 2])) {
      row(H / 2);
    }
    if (!equal(goal, goal + W, a[H / 2])) {
      GG;
    }
  }
  if (W % 2) {
    auto check = [&]() {
      for (int i = 0; i < H; ++i) {
        if (a[i][W / 2] != getID(i, W / 2)) {
          return false;
        }
      }
      return true;
    };
    if (!check()) {
      col(W / 2);
    }
    if (!check()) {
      GG;
    }
  }
  for (int i = 0; i < H / 2; ++i) {
    for (int j = 0; j < W / 2; ++j) {
      int i1 = H - 1 - i, j1 = W - 1 - j;
      vis[getID(i, j)] = 0;
      vis[getID(i, j1)] = 1;
      vis[getID(i1, j)] = 2;
      vis[getID(i1, j1)] = 3;
      S4[0] = vis[a[i][j]];
      S4[1] = vis[a[i][j1]];
      S4[2] = vis[a[i1][j]];
      S4[3] = vis[a[i1][j1]];
      if (count(S4, S4 + 4, -1)) {
        GG;
      }
      sgn[i][j] = SGN();
      vis[getID(i, j)] = -1;
      vis[getID(i, j1)] = -1;
      vis[getID(i1, j)] = -1;
      vis[getID(i1, j1)] = -1;
    }
  }
  for (int i = 0; i < W / 2; ++i) {
    c[i] = sgn[0][i];
  }
  for (int i = 1; i < H / 2; ++i) {
    r[i] = sgn[i][0] ^ c[0];
  }
  for (int i = 0; i < H / 2; ++i) {
    for (int j = 0; j < W / 2; ++j) {
      if (sgn[i][j] != (r[i] ^ c[j])) {
        GG;
      }
    }
  }
  for (int i = 0; i < H / 2; ++i) {
    if (r[i]) {
      row(i);
    }
  }
  for (int i = 0; i < W / 2; ++i) {
    if (c[i]) {
      col(i);
    }
  }
  auto commuteRC = [&](int i, int j) { row(i), col(j), row(i), col(j); };
  auto commuteCR = [&](int i, int j) { col(i), row(j), col(i), row(j); };
  real = false;
  for (int i = 0; i < H / 2; ++i) {
    for (int j = 0; j < W / 2; ++j) {
      int i1 = H - 1 - i, j1 = W - 1 - j;
      vis[getID(i, j)] = 0;
      vis[getID(i, j1)] = 1;
      vis[getID(i1, j)] = 2;
      vis[getID(i1, j1)] = 3;
      int perm = pid(vis[a[i][j]], vis[a[i][j1]], vis[a[i1][j]], vis[a[i1][j1]]);
      switch (perm) {
        case pid(0, 1, 2, 3):break;
        case pid(3, 2, 1, 0): commuteCR(j, i), commuteRC(i, W - 1 - j);
          break;
        case pid(1, 2, 0, 3):commuteCR(j, i);
          break;
        case pid(2, 0, 1, 3):commuteRC(i, j);
          break;
        case pid(1, 3, 2, 0):commuteRC(i, W - 1 - j);
          break;
        case pid(3, 0, 2, 1):commuteCR(W - 1 - j, i);
          break;
        case pid(2, 1, 3, 0):commuteCR(j, H - 1 - i);
          break;
        case pid(3, 1, 0, 2):commuteRC(H - 1 - i, j);
          break;
        case pid(0, 2, 3, 1):commuteRC(H - 1 - i, W - 1 - j);
          break;
        case pid(0, 3, 1, 2):commuteCR(W - 1 - j, H - 1 - i);
          break;
        case pid(1, 0, 3, 2):commuteRC(i, j), commuteCR(W - 1 - j, H - 1 - i);
          break;
        case pid(2, 3, 0, 1):commuteCR(j, i), commuteRC(H - 1 - i, W - 1 - j);
          break;
      }
    }
  }
  cout << "POSSIBLE " << ans.size();
  for (const auto &pr : ans) {
    cout << ' ' << pr.first << pr.second;
  }
  cout << '\n';
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  memset(vis, -1, sizeof(vis));
  int T;
  cin >> T;
  while (T--) {
    solve();
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

