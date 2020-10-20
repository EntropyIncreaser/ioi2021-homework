#include <bits/stdc++.h>

using namespace std;

bool check(int l, int r) {
  cout << "check " << l << ' ' << r << endl;
  string result;
  cin >> result;
  return result == "Yes";
}

int main() {
  int xl = 0, xr, vl = 0, vr;
  cin >> xr >> vr;
  vector<int> pl(100), pr(100);
  for (int t = 0; t < 100; ++t) {
    int l = 0, r = numeric_limits<int>::max();
    for (int i = 0; i < t; ++i) {
      l = max(l, pl[i] + (t - i) * vl);
      r = min(r, pr[i] + (t - i) * vr);
    }
    if (l == r && vl == vr) {
      cout << "answer " << l << endl;
      break;
    }
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
