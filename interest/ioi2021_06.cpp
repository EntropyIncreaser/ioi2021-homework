#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;

const int P = 998244353;

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

int norm(int x) { return x >= P ? x - P : x; }

void add(int &x, int y) {
  if ((x += y - P) < 0)
    x += P;
}

void sub(int &x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int fac[1 << 20], ifac[1 << 20];

void pre(int N) {
  fac[0] = 1;
  for (int n = 1; n <= N; ++n) fac[n] = fac[n - 1] * (ull) n % P;
  ifac[N] = mpow(fac[N], P - 2);
  for (int n = N; n; --n) ifac[n - 1] = ifac[n] * (ull) n % P;
}

int binom(int n, int m) { return fac[n] * (ull) ifac[m] % P * ifac[n - m] % P; }

int sbinom(int n, int m) { return (m & 1) ? (P - binom(n, m)) : binom(n, m); }

int query(int A, int B, int N) {
  if (A < B || N <= B)
    return 0;
  if (A == B) return 1;
  if (A < N) return 0;
  return sbinom(A - B - 1, N - B - 1) * (ull) binom(A, B) % P;
}

int solve(int N, int m, int k) {
  ++k;
  ++N;
  pre(m * (N + 1 - k));
  int ans = 0;
  for (int j = 1; j < N + 1 - k; ++j) {
    add(ans, query(m * j, j + k - 1, N));
    sub(ans, query(m * j, j + k, N));
  }
  return ans;
}

int main() {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, m, k;
  cin >> N >> m >> k;
  cout << solve(N, m, k) << '\n';

  return 0;
}
