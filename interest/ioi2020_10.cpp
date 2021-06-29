#include <bits/stdc++.h>

typedef unsigned long long ull;

const int P = 998244353;

int norm(int x) { return x >= P ? x - P : x; }
int reduce(int x) { return x < 0 ? x + P : x; }
int neg(int x) { return x ? P - x : 0; }
void add(int& x, int y) { if ((x += y - P) < 0) x += P; }
void sub(int& x, int y) { if ((x -= y) < 0) x += P; }
int mpow(int x, unsigned k) {
	if (k == 0) return 1;
	int ret = mpow(x * (ull)x % P, k >> 1);
	if (k & 1) ret = ret * (ull)x % P;
	return ret;
}

const int L = 20;

namespace NTT {

	const int OMEGA_2_21 = 31;

	int l, n;
	int w2[(1 << L) + 1];

	void setL(int l) {
		NTT::l = l;
		n = 1 << l;
	}

	void init() {
		*w2 = 1;
		w2[1 << L] = mpow(OMEGA_2_21, 1 << (21 - L));
		for (int i = L; i; --i) w2[1 << (i - 1)] = w2[1 << i] * (ull)w2[1 << i] % P;
		for (int i = 1; i != 1 << L; ++i) w2[i] = w2[i & (i - 1)] * (ull)w2[i & -i] % P;
	}

	void DIF(int *a) {
		int i, *j, *k, len = n >> 1, r, *o;
		for (i = 0; i < l; ++i, len >>= 1)
			for (j = a, o = w2; j != a + n; j += len << 1, ++o)
				for (k = j; k != j + len; ++k)
					r = *o * (ull)k[len] % P, k[len] = reduce(*k - r), add(*k, r);
	}

	void DIT(int *a) {
		int i, *j, *k, len = 1, r, *o;
		for (i = 0; i < l; ++i, len <<= 1)
			for (j = a, o = w2; j != a + n; j += len << 1, ++o)
				for (k = j; k != j + len; ++k)
					r = reduce(*k + k[len] - P), k[len] = ull(*k - k[len] + P) * *o % P, *k = r;
	}

	void FFT(int* a, int d = 1) {
		if (d == 1) DIF(a);
			else {
				DIT(a);
				std::reverse(a + 1, a + n);
				ull nv = P - (P - 1) / n;
				for (int i = 0; i < n; ++i) a[i] = a[i] * nv % P;
			}
	}

}

const int N = 500010;

int de[N], fac[N], ifac[N];
int fw[N];

void init(int n) {
	de[0] = 1;
	for (int i = 1; i <= n + 1; ++i) de[i] = (1 + (P - i) * (ull)de[i - 1]) % P;
	for (int i = 0; i <= n; ++i) sub(de[i], de[i + 1]);
	for (int i = 1; i <= n; i += 2) de[i] = neg(de[i]);
	fac[0] = 1;
	for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * (ull)i % P;
	ifac[n] = mpow(fac[n], P - 2);
	for (int i = n; i; --i) ifac[i - 1] = ifac[i] * (ull)i % P;
}

int x[1 << L], y[1 << L], qf[N], q1[N];

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);

	NTT::init();
	init(5e5);

	int T; std::cin >> T;
	while (T--) {
		int n, p, q; std::cin >> n >> p >> q;
		p = p * (ull)mpow(q, P - 2) % P;
		q = reduce(1 - p);
		int ans = 0;
		int lgn = 0; while ((1 << lgn) <= n) ++lgn; ++lgn;
		NTT::setL(lgn);
		memset(x, 0, sizeof(int) << lgn); memset(y, 0, sizeof(int) << lgn);
		int pw = q;
		qf[0] = 1;
		for (int i = 1; i <= n - 1; ++i) {
			pw = pw * (ull)q % P;
			q1[i] = reduce(1 - pw);
			qf[i] = qf[i - 1] * (ull)q1[i] % P;
		}
		int iv = mpow(qf[n - 1], P - 2);
		for (int i = n - 2; i >= 0; --i) {
			int v = iv * (ull)qf[i] % P;
			x[i] = de[i] * (ull)v % P;
			iv = iv * (ull)q1[i + 1] % P;
		}
		for (int i = 0; i <= n - 2; ++i) x[i] = x[i] * (ull)ifac[i] % P;
		std::copy(ifac, ifac + n - 1, y);
		NTT::FFT(x); NTT::FFT(y);
		for (int i = 0; i != 1 << lgn; ++i) x[i] = x[i] * (ull)y[i] % P;
		NTT::FFT(x, -1); std::fill(x + n - 1, x + (1 << lgn), 0);
		for (int i = 0; i <= n - 2; ++i) x[i] = x[i] * (ull)fac[i] % P;
		std::reverse(x, x + n - 1);
		pw = 1;
		for (int i = 0; i <= n - 2; ++i) {
			x[i] = x[i] * (ull)pw % P * ifac[i] % P;
			pw = pw * (ull)p % P;
		}
		memset(y, 0, sizeof(int) << lgn);
		pw = 1;
		for (int i = 0; i <= n - 2; ++i) {
			y[i] = ifac[i] * (ull)pw % P;
			pw = pw * (ull)q % P;
		}
		NTT::FFT(x); NTT::FFT(y);
		for (int i = 0; i != 1 << lgn; ++i) x[i] = x[i] * (ull)y[i] % P;
		NTT::FFT(x, -1); std::fill(x + n - 1, x + (1 << lgn), 0);
		for (int i = 0; i <= n - 2; ++i) x[i] = x[i] * (ull)fac[i] % P;
		std::fill(fw + 1, fw + n + 1, 0);
		for (int i = 1; i <= n; ++i) {
			int ai; std::cin >> ai;
			int l = 0;
			for (int j = ai; j; j &= j - 1) l += fw[j];
			int r = ai - 1 - l;
			if (i > 1) ans = (ans + l * (ull)x[i - 2] % P * q) % P;
			if (i < n) ans = (ans + r * (ull)x[i - 1]) % P;
			for (int j = ai; j <= n; j += j & -j) ++fw[j];
		}
		ans = (ans * (ull)p + 1) % P;
		std::cout << ans << '\n';
	}

	return 0;
}
