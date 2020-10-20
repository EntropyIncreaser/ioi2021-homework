#include <vector>
#include <iostream>
#include <set>

#include "testlib.h"

using namespace std;

int n;
string s;
set<string> st;

bool test(const string &a, const string &b) {
  if (a.empty())return true;
  int i = 0;
  for (int j = 0; j != b.size(); ++j) {
    if (a[i] == b[j])
      if (++i == a.length())
        return true;
  }
  return false;
};

void readChain(InStream& stream, set<string>& st1, int len) {
  string tail = "";
  while (len--) {
    string cur = stream.readString();
    if (!test(tail, cur)) {
      stream.quit(_wa, "not a subsequence");
    }
    if (!st1.erase(cur)) {
      if (st.count(cur)) {
        stream.quit(_wa, "string occured twice");
      } else {
        stream.quit(_wa, "string not in the given set");
      }
    }
    swap(tail, cur);
  }
  if (!test(tail, s)) {
    stream.quit(_wa, "not a subsequence");
  }
}

bool readAns(InStream& stream) {
  string str = stream.readToken();
  if (str == "impossible") {
    return false;
  }
  long long s1 = stringToLongLong(stream, str.c_str());
  if (s1 < 0 || s1 > n) {
    stream.quit(_wa, ("Integer parameter [name=s1] equals to " + vtos(s1) + ", violates the range [" + vtos(0) + ", " + vtos(n) + "]").c_str());
  }
  int s2 = stream.readInt(0, n, "s2");
  if (s1 + s2 != n) {
    stream.quit(_wa, ("s1 + s2 = " + vtos(s1) + " + " + vtos(s2) + " != " + vtos(n) + " = n").c_str());
  }
  set<string> st1 = st;
  stream.readEoln();
  readChain(stream, st1, s1);
  readChain(stream, st1, s2);
  return true;
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  n = inf.readInt();
  inf.readEoln();
  s = inf.readString();
  for (int i = 0; i < n; ++i) {
    st.insert(inf.readString());
  }

  bool pans = readAns(ouf), jans = readAns(ans);

  if (pans == jans)
    quitf(_ok, pans ? "OK found answer" : "OK impossible");
  else if (pans)
    quitf(_fail, "jury didn't find answer, but answer exists");
  else
    quitf(_wa, "participant didn't find answer, but answer exists");

  return 0;
}
