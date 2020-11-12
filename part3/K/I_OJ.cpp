#include <bits/stdc++.h>
#include "intercept.h"

using namespace std;

int intercept(int xr, int vr) {
  int xl = 0, vl = 0;
  vector<int> pl(100), pr(100);
  for (int t = 0; t < 100; ++t) {
    int l = 0, r = 1000000000;
    for (int i = 0; i < t; ++i) {
      l = max(l, pl[i] + (t - i) * vl);
      r = min(r, pr[i] + (t - i) * vr);
    }
    if (l == r && vl == vr)
      return l;
    int mid = (l + r) >> 1;
    if (check(l, mid))
      r = mid;
    else
      l = mid + 1;
    pl[t] = l;
    pr[t] = r;
    for (int i = 0; i < t; ++i) {
      vl = max(vl, (l - pr[i] + (t - i - 1)) / (t - i));
      vr = min(vr, (r - pl[i]) / (t - i));
    }
  }
  return 0;
}
