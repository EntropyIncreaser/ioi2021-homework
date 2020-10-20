# Fygon 2.0

**题目来源：https://codeforces.com/gym/101612/ F**

### 题目描述

深受喜爱的编程语言 Fygon 的新版本已经发布！全新的 Fygon 2.0 仍然只有两种语句。第一种语句是 $\texttt{lag}$，它几乎可以代替任何其他语句。第二章语句是 $\texttt{for}$ 循环：

```
for <variable> in range(<from>, <to>):
    <body>
```

- 该 $\texttt{for}$ 循环将 $\texttt{&lt;variable&gt;}$ 变量从 $\texttt{&lt;from&gt;}$ 迭代到 $\texttt{&lt;to&gt;}$，**两端均包含**。
- 如果 $\texttt{&lt;from&gt;}$ 的值大于 $\texttt{&lt;to&gt;}$，那么 $\texttt{&lt;body&gt;}$ 完全不会被执行。
- $\texttt{&lt;variable&gt;}$ 是一个 $\texttt{a}$ 到 $\texttt{z}$ 之间的字符但不是 $\texttt{n}$，$\texttt{n}$ 是一个在当前代码片段之前已经被定义的变量。
- $\texttt{&lt;from&gt;}$ 和 $\texttt{&lt;to&gt;}$ 可以是外部循环中任何一个变量，特别的，$\texttt{&lt;from&gt;}$ 还可以是 $\texttt{1}$，$\texttt{&lt;to&gt;}$ 还可以是 $\texttt{n}$。
- 循环中执行的 $\texttt{&lt;body&gt;}$ 是 $4$ 空格缩进的，且至少有一个语句。

如果你熟悉 Fygon 1.0，你会注意到，本着最佳编程实践的精神，Fygon 2.0 是不向后兼容的，因为现在的 $\texttt{range}$ 函数需要两个参数。

新版本的性能有了明显的提高，所以你可以写更多的嵌套 $\texttt{for}$ 循环。这就是为什么我们不再关心确切的操作次数，而是关心程序的渐进复杂度。为了简单起见，所有的 `for` 循环都是一连串嵌套的，而且在所有 $\texttt{for}$ 循环的最里面恰好有一个 $\texttt{lag}$ 语句。所有的循环变量都是不同的，并且都不是 $\texttt{n}$。

让我们定义 $f(n)$ 为一个给定的 Fygon 程序执行的 $\texttt{lag}$ 次数，关于 $n$ 的函数。对于非负整数 $k$ 和正有理数 $C$，我们说 $C\cdot n^k$ 是程序的渐近复杂度，当：

$$
\lim_{n\rightarrow \infty} \frac{f(n)}{C\cdot n^k} =1
$$

给你一个 Fygon 2.0 程序，求其渐近复杂度。

### 输入格式

输入的第一行包含 Fygon 2.0 程序的行数 $m$。接下来的 $m$ 行即程序本身。程序至少有 $1$ 个，最多 $20$ 个 $\texttt{for}$ 语句。每个 $\texttt{for}$ 包含单个嵌套 $\texttt{for}$ 语句或 $\texttt{lag}$ 语句。

### 输出格式

输出数 $k$ 和 $C$。$C$ 应以不可约分数 $p/q$ 的形式输出，其中 $p$ 和 $q$ 互素。

### 样例数据

#### 输入

```
4
for i in range(1, n):
    for j in range(1, i):
        for k in range(j, n):
            lag
```

#### 输出

```
3 1/3
```

### 时空限制

时间限制：3s

空间限制：512MB
