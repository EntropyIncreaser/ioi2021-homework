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

const int N = 2010;

namespace LCT {

struct Node {
  bool rev;
  int sz;
  union {
    struct {
      Node *ls, *rs;
    };
    Node *ch[2];
  };
  Node *prt, *pth, *virt;

  bool rel() const { return this == prt->rs; }

  void upd() {
    sz = 1;
    for (int i = 0; i < 2; ++i)
      if (ch[i])
        sz += ch[i]->sz;
  }

  void pd() {
    if (rev) {
      swap(ls, rs);
      for (int i = 0; i < 2; ++i)
        if (ch[i])
          ch[i]->rev ^= true;
      rev = false;
    }
  }

  void rot() {
    prt->pd();
    pd();
    bool f = rel();
    pth = prt->pth;
    prt->pth = nullptr;
    Node *p = prt;
    prt = p->prt;
    if (prt)
      prt->ch[p->rel()] = this;
    p->ch[f] = ch[!f];
    if (ch[!f]) ch[!f]->prt = p;
    ch[!f] = p;
    p->prt = this;
    p->upd();
    upd();
  }

  void spl(Node *goal = nullptr) {
    while (prt != goal) {
      if (prt->prt == goal) return rot();
      prt->prt->pd();
      prt->pd();
      if (rel() == prt->rel()) {
        prt->rot();
        rot();
      } else {
        rot();
        rot();
      }
    }
  }

  void expose() {
    spl();
    pd();
    if (rs) {
      rs->pth = this;
      rs->prt = nullptr;
      rs = nullptr;
      upd();
    }
  }

  bool splice() {
    spl();
    if (!pth) return false;
    pth->expose();
    pth->rs = this;
    prt = pth;
    pth = nullptr;
    prt->upd();
    return true;
  }

  void access() {
    expose();
    while (splice());
  }

  void evert() {
    access();
    rev ^= true;
  }
} P[N * N];

void init(Node *u) {
  u->sz = 1;
}

void link(Node *u, Node *v) {
  u->evert();
  u->pth = v;
}

void cut(Node *u, Node *v) {
  u->spl(nullptr);
  v->access();
  u->pth = nullptr;
}

int query(Node *u) {
  u->access();
  return u->sz;
}

Node *kth(Node *o, int k) {
  while (true) {
    o->pd();
    int lc = o->ls ? o->ls->sz : 0;
    if (k <= lc) {
      o = o->ls;
    } else {
      k -= lc;
      if (k == 1) {
        break;
      }
      --k;
      o = o->rs;
    }
  }
  o->spl(nullptr);
  return o;
}

Node *getRoot(Node *u) {
  u->access();
  while (u->pd(), u->ls) {
    u = u->ls;
  }
  u->spl(nullptr);
  return u;
}

void addE(Node *u, Node *v) {
  if (getRoot(v) == u) {
    u->virt = v;
  } else {
    link(u, v);
  }
}

void cutE(Node *u, Node *v) {
  v->access();
  u->spl(nullptr);
  if (u->pth == v) {
    u->pth = nullptr;
    u = getRoot(v);
    Node *p = u->virt;
    u->virt = nullptr;
    if (p) {
      addE(u, p);
    }
  } else {
    u->virt = nullptr;
  }
}

Node *moveK(Node *u, int k) {
  int len = query(u);
  if (k < len) {
    u->access();
    return kth(u, len - k);
  }
  k -= len;
  u = getRoot(u)->virt;
  len = query(u);
  k %= len;
  u->access();
  return kth(u, len - k);
}

}

using LCT::P;

int r, c;
int a[N][N];

int getId(int x, int y) {
  return x * c + y;
}

int getX(int id) {
  return id / c;
}

int getY(int id) {
  return id % c;
}

template<bool TYPE>
void ins(int x, int y) {
  pair<int, int> best(0, 0);
  best = max(best, make_pair(a[(x + r - 1) % r][(y + 1) % c], (x + r - 1) % r));
  best = max(best, make_pair(a[x][(y + 1) % c], x));
  best = max(best, make_pair(a[(x + 1) % r][(y + 1) % c], (x + 1) % r));
  auto u = P + getId(x, y), v = P + getId(best.second, (y + 1) % c);
  if (TYPE) {
    LCT::addE(u, v);
  } else {
    LCT::cutE(u, v);
  }
}

int main() {
#ifdef ELEGIA
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> r >> c;
  for (int i = 0; i < r * c; ++i) {
    LCT::init(P + i);
  }
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      cin >> a[i][j];
    }
  }
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      ins<1>(i, j);
    }
  }
  int px = 0, py = 0;
  int q;
  cin >> q;
  while (q--) {
    string opt;
    cin.ignore();
    cin >> opt;
    if (opt == "move") {
      int k;
      cin >> k;
      int id = LCT::moveK(P + getId(px, py), k) - P;
      px = getX(id);
      py = getY(id);
      cout << px + 1 << ' ' << py + 1 << '\n';
    } else {
      int x, y, z;
      cin >> x >> y >> z;
      --x;
      --y;
      ins<0>((x + r - 1) % r, (y + c - 1) % c);
      ins<0>(x, (y + c - 1) % c);
      ins<0>((x + 1) % r, (y + c - 1) % c);
      a[x][y] = z;
      ins<1>((x + r - 1) % r, (y + c - 1) % c);
      ins<1>(x, (y + c - 1) % c);
      ins<1>((x + 1) % r, (y + c - 1) % c);
    }
  }

#ifdef ELEGIA
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}

