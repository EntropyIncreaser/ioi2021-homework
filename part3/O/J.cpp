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
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <random>
#include <bitset>
#include <queue>
#include <functional>
#include <set>
#include <map>
#include <vector>
#include <chrono>
#include <iostream>
#include <limits>
#include <numeric>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
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

#ifdef ELEGIA
const int N = 1010, M = 1010;
#else
const int N = 1010, M = 100010;
#endif

namespace SEG {

struct Node {
  int ls, rs;
  ll s;
} P[M * 42];

int top;

void clear() {
  memset(P + 1, 0, sizeof(Node) * top);
  top = 0;
}

int build(int l, int r) {
  int o = ++top;
  if (l == r) return o;
  int mid = (l + r) >> 1;
  P[o].ls = build(l, mid);
  P[o].rs = build(mid + 1, r);
  return o;
}

int change(int o, int l, int r, int k, int x) {
  int p = ++top;
  memcpy(P + p, P + o, sizeof(Node));
  if (l == r) {
    P[p].s = x;
    return p;
  }
  int mid = (l + r) >> 1;
  if (k <= mid)P[p].ls = change(P[o].ls, l, mid, k, x);
  else P[p].rs = change(P[o].rs, mid + 1, r, k, x);
  P[p].s = P[P[p].ls].s + P[P[p].rs].s;
  return p;
}

ll query(int o, int l, int r, int k) {
  if (k == r)return P[o].s;
  int mid = (l + r) >> 1;
  return k <= mid ? query(P[o].ls, l, mid, k) : (P[P[o].ls].s + query(P[o].rs, mid + 1, r, k));
}

}

struct LCT {
  struct Node {
    bool rev;
    int val, mx;
    union {
      struct {
        Node *ls, *rs;
      };
      Node *ch[2];
    };
    Node *prt, *path;

    bool relation() const { return this == prt->rs; }

    void pushDown() {
      if (rev) {
        rev = false;
        swap(ls, rs);
        if (ls)
          ls->rev = !ls->rev;
        if (rs)
          rs->rev = !rs->rev;
      }
    }

    void update() {
      mx = val;
      if (ls)
        mx = max(mx, ls->mx);
      if (rs)
        mx = max(mx, rs->mx);
    }

    void rotate() {
      if (prt->prt)
        prt->prt->pushDown();
      prt->pushDown();
      pushDown();
      path = prt->path;
      prt->path = NULL;
      bool f = relation();
      Node *p = prt;
      if (p->prt)
        p->prt->ch[p->relation()] = this;
      prt = p->prt;
      p->ch[f] = ch[!f];
      if (ch[!f])
        ch[!f]->prt = p;
      ch[!f] = p;
      p->prt = this;
      p->update();
      update();
    }

    void splay() {
      while (prt) {
        if (!prt->prt)
          return rotate();
        prt->prt->pushDown();
        prt->pushDown();
        if (relation() == prt->relation()) {
          prt->rotate();
          rotate();
        } else {
          rotate();
          rotate();
        }
      }
    }

    void expose() {
      splay();
      pushDown();
      if (rs) {
        rs->path = this;
        rs->prt = NULL;
        rs = NULL;
        update();
      }
    }

    bool splice() {
      splay();
      if (!path)
        return false;
      path->expose();
      path->rs = this;
      prt = path;
      path = NULL;
      prt->update();
      return true;
    }

    void access() {
      expose();
      while (splice());
    }

    void evert() {
      access();
      splay();
      rev = !rev;
    }

    int query() {
      access();
      splay();
      return mx;
    }

    Node *top() {
      access();
      splay();
      Node *p = this;
      while (p->pushDown(), p->ls)
        p = p->ls;
      p->splay();
      return p;
    }
  };

  Node ver[N + M];

  void link(int u, int v) {
    ver[u].evert();
    ver[u].path = ver + v;
  }

  void cut(int u, int v) {
    ver[u].evert();
    ver[v].access();
    ver[u].expose();
    ver[v].path = NULL;
  }

  int query(int u, int v) {
    ver[u].evert();
    return ver[v].query();
  }

  bool conn(int u, int v) {
    return ver[u].top() == ver[v].top();
  }
} mst;

int n, m;
tuple<int, int, int> ed[M];
int key[M];
int rt[M];

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 1; i <= m; ++i) {
      int u, v, w;
      cin >> u >> v >> w;
      ed[i] = tie(w, u, v);
    }
    sort(ed + 1, ed + m + 1);
    rt[m + 1] = SEG::build(1, m);
    for (int i = m; i; --i) {
      int u, v, w;
      tie(w, u, v) = ed[i];
      key[i] = w;
      rt[i] = SEG::change(rt[i + 1], 1, m, i, w);
      mst.ver[n + i].val = i;
      mst.ver[n + i].update();
      if (mst.conn(u, v)) {
        int j = mst.query(u, v);
        rt[i] = SEG::change(rt[i], 1, m, j, 0);
        mst.cut(get<1>(ed[j]), n + j);
        mst.cut(get<2>(ed[j]), n + j);
      }
      mst.link(u, n + i);
      mst.link(v, n + i);
    }
    ll ans = 0;
    int q;
    cin >> q;
    while (q--) {
      ll l, r;
      cin >> l >> r;
      l -= ans;
      r -= ans;
      int lp = lower_bound(key + 1, key + m + 1, l) - key,
              rp = upper_bound(key + 1, key + m + 1, r) - key - 1;
      if (rp >= 1 && rp <= m)
        ans = SEG::query(rt[lp], 1, m, rp);
      else ans = 0;
      cout << ans << '\n';
    }
    memset(mst.ver + 1, 0, sizeof(LCT::Node) * (n + m));
    SEG::clear();
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
