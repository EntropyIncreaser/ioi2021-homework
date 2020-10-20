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

#ifdef LBT
const int N = 100;
#else
const int N = 1000010;
#endif

struct Node {
  int s;
  Node *ch[26], *fail;

  void*operator new(size_t) {
    static Node *ptop;
    if (!ptop) {
      ptop = static_cast<Node *>(malloc(N * 2 * sizeof(Node)));
      memset(ptop, 0, N * 2 * sizeof(Node));
    }
    return ptop++;
  }
};

int n, m;
char s[N];
Node *pos[N], *q[N * 2];
Node* trie;

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif

  scanf("%d%d", &n, &m);
  pos[0] = trie = new Node;
  for (int i = 1; i <= n; ++i) {
    char c;
    int prt;
    scanf(" %c%d", &c, &prt);
    pos[i] = pos[prt]->ch[c - 'A'] = new Node;
    ++pos[i]->s;
  }
  for (int i = 1; i <= m; ++i) {
    scanf("%s", s);
    Node* t = trie;
    int l = strlen(s);
    for (char* p = s + l - 1; p >= s; --p) {
      if (t->ch[*p - 'A'])
        t = t->ch[*p - 'A'];
      else
        t = t->ch[*p - 'A'] = new Node;
    }
    pos[i] = t;
  }
  int qr = 0, ql = 0;
  q[qr++] = trie;
  trie->fail = trie;
  while (ql < qr) {
    Node* p = q[ql++];
    for (int i = 0; i < 26; ++i)
      if (p->ch[i]) {
        q[qr++] = p->ch[i];
        if (p == trie) {
          p->ch[i]->fail = trie;
        } else {
          p->ch[i]->fail = p->fail->ch[i];
        }
      } else if (p == trie)
        p->ch[i] = trie;
      else
        p->ch[i] = p->fail->ch[i];
  }
  for (int i = qr - 1; i; --i)
    q[i]->fail->s += q[i]->s;
  for (int i = 1; i <= m; ++i)
    printf("%d\n", pos[i]->s);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
