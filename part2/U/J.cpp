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

const int N = 100010, K = 11;
const int Mod = 1000000000;

int norm(int x) { return x >= Mod ? x - Mod : x; }

void add(int &x, int y) {
  if ((x += y) >= Mod)
    x -= Mod;
}

void sub(int &x, int y) {
  if ((x -= y) < 0)
    x += Mod;
}

int n;
int xi[N], pw[N];
int kSum[K];
char s[N];

vi vecX() {
  vi ret(K);
  ret[1] = 1;
  return ret;
}

vi scalar(int v) {
  vi ret(K);
  ret[0] = v;
  return ret;
}

vi neg(vi vec) {
  for (int& x : vec)
    x = norm(Mod - x);
  return vec;
}

vi add(vi lhs, const vi& rhs) {
  for (int i = 0; i < K; ++i)
    add(lhs[i], rhs[i]);
  return lhs;
}

vi mul(const vi& lhs, const vi& rhs) {
  vi ret(K);
  for (int i = 0; i < K; ++i) {
    ull v = 0;
    for (int j = 0; j <= i; ++j)
      v += lhs[j] * (ull) rhs[i - j];
    ret[i] = v % Mod;
  }
  return ret;
}

int fold(const vi& vec) {
  ull v = 0;
  for (int i = 0; i < K; ++i)
    v += vec[i] * (ull)kSum[i];
  return v % Mod;
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
#ifdef ONLINE_JUDGE
  freopen("j.in", "r", stdin);
  freopen("j.out", "w", stdout);
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < n; ++i) cin >> xi[i];
  cin >> (s + 1);
  s[0] = '(';
  fill_n(pw, n, 1);
  for (int i = 0; i < K; ++i) {
    kSum[i] = accumulate(pw, pw + n, 0LL) % Mod;
    for (int j = 0; j < n; ++j) pw[j] = pw[j] * (ull) xi[j] % Mod;
  }
  int len = strlen(s);
  s[len++] = ')';
  stack<vi> value;
  stack<string> op;
  value.push(vi());
  op.push("");
  auto applyValue = [&](const vi& val) {
    if (op.top() == "") {
      value.top() = val;
    } else if (op.top() == "+") {
      value.top() = add(value.top(), val);
    } else if (op.top() == "-") {
      value.top() = add(neg(value.top()), val);
    } else if (op.top() == "*") {
      value.top() = mul(value.top(), val);
    }
    op.top() = "";
  };
  auto holdNeg = [&]() {
    if (op.top() == "-") {
      value.top() = neg(value.top());
      op.top() = "";
    }
  };
  auto holdEnd = [&]() {
    holdNeg();
    auto val = value.top();
    value.pop();
    op.pop();
    applyValue(val);
  };
  for (int i = len - 1; i; --i) {
    if (s[i] == ')') {
      value.push(vi());
      op.push("");
    } else if (s[i] == '(') {
      holdEnd();
    } else if (s[i] == 'N') {
      applyValue(scalar(n));
    } else if (s[i] == 'X') {
      applyValue(vecX());
    } else if (isdigit(s[i])) {
      int power = 1, val = s[i] - '0';
      while (isdigit(s[i - 1])) {
        power *= 10;
        val += (s[--i] - '0') * power;
      }
      applyValue(scalar(val));
    } else if (s[i] == '+' || s[i] == '-' || s[i] == '*') {
      holdNeg();
      op.top() = s[i];
    } else if (s[i] == '/') {
      holdNeg();
      value.top() = scalar(fold(value.top()));
      --i;
    } else if (s[i] == ':') {
      holdNeg();
      value.top() = mul(value.top(), value.top());
      --i;
    }
  }
  holdEnd();
  cout << value.top()[0] << '\n';

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

