由于不太会析合树，我们考虑一个分治做法。记 $I(x, y)$ 是 $[\min(x,y),\max(x,y)]$ 的本原连续段。取 $m=\lfloor \frac{l+r}2\rfloor$，我们考虑如何计算对于 $l\le k\le r$，满足 $I(k,m) \subseteq [l,r]$ 的 $k$ 计算出 $I(k,m)$，这样若 $m\in I(a,b)\subseteq [l,r]$，那么必有 $I(a,b) = I(a,m) \cup I(b,m)$。容易发现在 $k$ 从 $m$ 减小至 $l$ 的过程中区间是单调的，我们用几个指针维护一下即可做到线性算出所有 $I(k,m)$。因此就得到了一个 $\Theta(n\log n + q\log n)$ 复杂度的算法。

