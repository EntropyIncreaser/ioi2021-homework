我们可以通过容斥得到 PGF，设 $p=\frac 13$，则有
$$
f(x)=\frac{1+B(px)}{(1-x)(1+B(px))+(px)^L}
$$
其中 $B(x) = \sum_{k\in Border(S)} x^{L-k}$。

我们做一个差分，就有
$$
(1-x)f(x)=\frac1{1+\dfrac{(px)^L}{(1-x)(1+B(px))}}
$$
由于 $p=\frac 13$，所以这个东西在分析大小关系的时候，我们考虑
$$
(1-x)f(x) = 1 - \frac{(px)^L}{(1-x)(1+B(px))} + \left(\frac{(px)^L}{(1-x)(1+B(px))}\right)^2 - \cdots
$$
注意到这个东西带着一个 $p^{kL}$ 的缩放系数，而下面的递推均是 $p^{L-k}$ 的故比不过 $p^k$，我们可以做近似
$$
(1-x)f(x) \approx 1-\frac{(px)^L}{(1-x)(1+B(px))}
$$
我们在比较第 $m$ 位的时候，注意到 Border 中 $2L-k>m$ 的都是没有被用到的。<del> 于是大概就去比较剩下部分的字典序就好了 </del>

然后我们跑一下 KMP 就可以了。