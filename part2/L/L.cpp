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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

ostream& operator<<(ostream& os, const pair<char, int>& unit) {
  return os << unit.first << "^" << unit.second;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

const int N = 4100, M = 6100;

int A[N][N], B[M][M];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  while (n--) {
    char c;
    int x, y, d;
    cin.ignore();
    cin >> c >> x >> y >> d;
    if (c == 'A') {
      x += N / 2;
      y += N / 2;
      ++A[x - d / 2][y - d / 2];
      --A[x + d / 2][y - d / 2];
      --A[x - d / 2][y + d / 2];
      ++A[x + d / 2][y + d / 2];
    } else {
      int u = x + y, v = x - y;
      u += M / 2;
      v += M / 2;
      ++B[u - d / 2][v - d / 2];
      --B[u - d / 2][v + d / 2];
      --B[u + d / 2][v - d / 2];
      ++B[u + d / 2][v + d / 2];
    }
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 1; j < N; ++j) {
      A[i][j] += A[i][j - 1];
    }
  }
  for (int j = 0; j < N; ++j) {
    for (int i = 1; i < N; ++i) {
      A[i][j] += A[i - 1][j];
    }
  }
  for (int i = 0; i < M; ++i) {
    for (int j = 1; j < M; ++j) {
      B[i][j] += B[i][j - 1];
    }
  }
  for (int j = 0; j < M; ++j) {
    for (int i = 1; i < M; ++i) {
      B[i][j] += B[i - 1][j];
    }
  }
  int ans = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (A[i][j]) {
        ans += 4;
      } else {
        int x = i + j - N + M / 2, y = i - j + M / 2;
        if (x < 2 || y < 2 || x > M - 2 || y > M - 2) {
          continue;
        }
        if (B[x][y]) {
          ++ans;
        }
        if (B[x][y - 1]) {
          ++ans;
        }
        if (B[x + 1][y]) {
          ++ans;
        }
        if (B[x + 1][y - 1]) {
          ++ans;
        }
      }
    }
  }
  cout << fixed << setprecision(2) << ans * .25 << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

