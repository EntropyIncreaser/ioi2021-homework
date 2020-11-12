#include <cstdio>
#include <cstdlib>

#include "intercept.h"

#define NAME(PARAM) /* SECRET */PARAM
#define TOKEN "/* SECRET */\n"

static int NAME(p), NAME(v), NAME(cnt);

bool check(int L, int R) {
  if (L < 0 || R > 1000000000 || L > R) {
    printf(TOKEN"ERROR_CHECK(%d,%d)", L, R);
    exit(0);
  }
  if (++NAME(cnt) > 100) {
    puts(TOKEN"TOO_MANY_CHECK");
    exit(0);
  }
  bool res = NAME(p) >= L && NAME(p) <= R;
  NAME(p) += NAME(v);
  return res;
}

int main() {
  int n, m;
  scanf("%d%d%d%d", &n, &m, &NAME(p), &NAME(v));
  NAME(p) = (NAME(p) + 239) % (n + 1);
  NAME(v) = (NAME(v) + 239) % (m + 1);
  int result = intercept(n, m);
  if (result == NAME(p)) printf(TOKEN"OK %d\n", NAME(cnt));
  else puts(TOKEN"WA");

  return 0;
}

#undef NAME
#undef TOKEN
