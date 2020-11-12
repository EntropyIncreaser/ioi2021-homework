**题目来源：https://www.codeforces.com/gym/100307 I**

**这是一道交互题。**

### 题目描述

东北紧急火箭控制机构（North Eastern Emergency Rocket Control agency, NEERC）开发了一种新的雷达控制系统，旨在更好地控制弹道火箭的拦截。为了测试新系统，NEERC 机构开发了一个数学模型，旨在显示这个系统的能力。最初该点位于 $0$ 和 $p$ 之间（含）的某个未知整数位置。它有一个未知的速度 $q$，是 $0$ 和 $v$ 之间（含）的整数。首先，控制系统向雷达发出 "$\texttt{check}\ L\ R$"的查询，得到该点目前是否在 $L$ 到 $R$（含）之间的答案，之后，该点的坐标增加 $q$，雷达控制系统的目标是在某一秒的开始检测该点的准确位置。当它完全确定了点的位置，那么它就不是向雷达进行查询，而是发出命令在该位置拦截该点。你必须实现控制系统定位和拦截该点，同时最多向雷达进行 $100$ 次查询。

### 实现细节

你不需要，也不应该实现主函数，你只需要实现函数 $\texttt{intercept}(p, v)$，其中 $p,v$ 的含义如题目描述所示。

你可以通过 $\texttt{check}(L, R)$ 函数来和交互库进行交互：

- 对雷达进行查询，该函数返回一个布尔值表示该点目前是否在 $L$ 到 $R$ 之间。在查询结果得出之后，该点的坐标会增加 $q$。
- 你需要保证 $0\le L\le R\le 10^9$。
- 该函数只能调用最多 $100$ 次。

你的函数需要返回一个值 $x$，表示此时该点的坐标。

### 实现方法

你可以参照 [$\texttt{template_intercept.cpp}$](https://paste.ubuntu.com/p/nHf5pZTCzB/)，将其重命名为 `intercept.cpp`，然后在其基础上答题。

- 请确保你的程序开头有 `#include "intercept.h"`。
- 你需要实现的函数 $\texttt{intercept}$ 的接口信息如下：
  - $\texttt{int intercept(int p, int v);}$
- 你可以调用的交互函数的接口如下：
  - $\texttt{bool check(int L, int R);}$

### 样例

<table class="table table-bordered"><thead><tr><th></th><th>$\texttt{intercept(2,2)}$</th></tr></thead><tbody><tr><td>$\texttt{check(1,3)}$</td><td>$\texttt{true}$</td></tr><tr><td>$\texttt{check(3,5)}$</td><td>$\texttt{false}$</td></tr><tr><td>$\texttt{check(2,4)}$</td><td>$\texttt{true}$</td></tr><tr><td>$\texttt{check(4,5)}$</td><td>$\texttt{true}$</td></tr><tr><td>$\texttt{return 5}$</td><td></td></tr></tbody></table>

本样例中，该点初始位于 $1$ 坐标，且速度为 $q=1$。

### 时空限制

时间限制：4s

空间限制：256MB
