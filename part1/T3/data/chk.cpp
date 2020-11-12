#include <vector>
#include <iostream>
#include <set>

#include "testlib.h"

using namespace std;

const char* TOKEN = "/* SECRET */";

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  string token = ouf.readLine();
  if (token != TOKEN) quitf(_fail, "wrong token");
  string str = ouf.readWord();
  if (str == "OK") {
    int cnt = ouf.readInt(0, 100);
    quitf(_ok, "%d checks", cnt);
  } else quitf(_wa, str.c_str());

  return 0;
}
