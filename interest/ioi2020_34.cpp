#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;


struct IO_Tp {
  const static int _I_Buffer_Size = 50 << 20;
  char _I_Buffer[_I_Buffer_Size], *_I_pos = _I_Buffer;

  const static int _O_Buffer_Size = 1 << 20;
  char _O_Buffer[_O_Buffer_Size], *_O_pos = _O_Buffer;

  IO_Tp() {
#ifdef ELEGIA
    freopen("test.in", "r", stdin);
#endif
    fread(_I_Buffer, 1, _I_Buffer_Size, stdin);
  }

  ~IO_Tp() {
    fwrite(_O_Buffer, 1, _O_pos - _O_Buffer, stdout);
  }

  IO_Tp &operator>>(int &res) {
    while (!isdigit(*_I_pos)) ++_I_pos;
    res = *_I_pos++ & 15;
    while (isdigit(*_I_pos)) res = res * 10 + (*_I_pos++ & 15);
    return *this;
  }

  IO_Tp &operator<<(int n) {
    static char _buf[20];
    char *_pos = _buf;
    do
      *_pos++ = '0' + n % 10;
    while (n /= 10);
    while (_pos != _buf) *_O_pos++ = *--_pos;
    return *this;
  }

  IO_Tp &operator<<(char ch) {
    *_O_pos++ = ch;
    return *this;
  }
} IO;

const int P = 998244353;

int norm(int x) { return x >= P ? (x - P) : x; }

void add(int &x, int y) { if ((x += y) >= P) x -= P; }

void sub(int &x, int y) { if ((x -= y) < 0) x += P; }

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ull) x % P;
    x = x * (ull) x % P;
    k >>= 1;
  }
  return ret;
}

const int N = 2000010;

int n;
vector<int> g[N];

struct Node {
  // edge, node
  int v[2][2];

  Node() {
    memset(v, 0, sizeof(v));
    v[0][0] = 1;
  }

  Node operator+(const Node &rhs) const {
    Node ret = Node();
    for (int i = 0; i <= 1; ++i)
      for (int j = 0; j <= 1; ++j)
        ret.v[i][j] = norm(v[i][j] + rhs.v[i][j]);
    return ret;
  }

  Node operator*(const Node &rhs) const {
    Node ret = Node();
    for (int i = 0; i <= 1; ++i)
      for (int j = 0; j <= 1; ++j) {
        ull x = 0;
        for (int s = 0; s <= i; ++s)
          for (int t = 0; t <= j; ++t)
            x += v[s][t] * (ull) rhs.v[i - s][j - t];
        ret.v[i][j] = x % P;
      }
    return ret;
  }

  Node operator*(int x) const {
    Node ret = Node();
    for (int i = 0; i <= 1; ++i)
      for (int j = 0; j <= 1; ++j) ret.v[i][j] = v[i][j] * (ull) x % P;
    return ret;
  }

  Node addN() const {
    Node ret = *this;
    for (int i = 0; i <= 1; ++i) add(ret.v[i][1], ret.v[i][0]);
    return ret;
  }

  Node dash() const {
    Node ret = *this;
    for (int i = 0; i <= 1; ++i) {
      ret.v[i][0] = ret.v[i][1] * (ull) n % P;
      ret.v[i][1] = 0;
    }
    return ret;
  }
} f[N];

bool vis[N];

Node dfs(int u) {
  vis[u] = true;
  Node ret = Node();
  for (int v : g[u])
    if (!vis[v]) {
      Node res = dfs(v);
      Node fa = res.dash() + res * (P - 1);
      res = res * 2;
      for (int i = 0; i <= 1; ++i) add(res.v[1][i], res.v[0][i]);
      ret = ret * (res + fa);
    }
  return ret.addN();
}

int main() {
  IO >> n;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    IO >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  int ans = dfs(1).v[1][1] * (ull)mpow(n, P - 2) % P;
  IO << ans << '\n';

  return 0;
}
