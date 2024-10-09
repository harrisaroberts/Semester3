# 1. Determine whether the following proofs are correct. Why or why not?
## (a) Theorem: If $n$ is an even integer, then $n^2$ is an even integer.
##     Proof. Since this theorem discusses an even integer $n$, we pick 2 to be our even
##     integer for this proof. Clearly $2 = 2 \cdot 1$, so 2 is even. Next, we see that $2^2 = 4$ is
##     also clearly even. Thus, for any even integer $n$, $n^2$ is an even integer.
This proof is wrong, it does not show that $n^2$ is an even integer for all integers and only uses one example.
## (b) Theorem: For any integer $n$, if $n^3$ is an odd integer, than $n$ is an odd integer.
##     Proof. We suppose that $n^3$ is an odd integer. By definition, $n^3 = 2k + 1$ for some
##     integer $k$. Next, we let $n = 2j + 1$, for some integer $j$. By definition, this shows
##     that $n$ is an odd integer.
This proof is also wrong, showing $n^3=2k+1$ does not show how they make the solution of $n=2j+1$
# 2. Consider the statement “If $3n + 2$ is even for some integer $n$, then $n$ is even”. Prove this statement using the given proof technique.
## (a) Proof by contrapositive
Assuming n is odd, n can be shown as $n=2k+1$ for some integer k. Substituting $n$ into $3n+2$ gives us $6k+5$. This is not divisible by two, so its not even. Therefore, if $n$ is odd, $3n+2$ is odd.
## (b) Proof by contradiction
Assuming that $3n+2$ is even for some integer $n$, but $n$ is odd. If $n$ is odd then $n=2k+1$ for some integer $k$. Substituting $n$ into $3n+2$ we get $6k+5$ which is odd. However for some integer $n$ $3n+2$ is even leading to a contradiction, so $n$ must be even.
# 3. Prove the following statements.
## (a) Use a proof by contradiction to show that if $r$ is a non-zero rational number and
Assuming that $r$ is a non-zero rational number and $s$ is an irrational numbers, then $rs$ is rational. $rs$ can be expressed as $rs=\frac{c}{d}$ where $c$ and $d$ are integers with no common factors and $d\neq 0$. Then, $rs=\frac{a}{b}\cdot s$, and $s=c\cdot\frac{b}{d}\cdot a$. Since $a,b,c,$ and $d$ are all integers this contradicts the assumption that s is irrational. So, if $r$ is a non-zero rational number and $s$ is an irrational number, then $rs$ must be irrational.$
## s is an irrational number, then $rs$ is irrational. Recall that a real number $r$ is
## rational if it can be written as $r =\frac{a}{b}$, where $a$ and $b$ are integers with no common
## factors and $b\neq 0$.
## (b) Show that if $a$ and $b$ are distinct rational numbers, then there is an irrational number between $a$ and $b$.
If $m=\frac{1}{2}$, then the value $a+\frac{1}{2}\cdot(b-a)$ represents a number exactly halfway between $a$ and $b$. $c=\frac{1}{2}a+\frac{1}{2}b$. Assuming the contradiction, that $c$ is a rational number and can be expressed as $\frac{p}{q}$ where $p$ and $q$ are integers with no common factors and $q\neq 0$. But if $\frac{1}{2}a+\frac{1}{2}b=\frac{p}{q}$, showing that $a+b=2\frac{p}{q}$ or $a+b=2c$, which must be rational. However, this contradicts that $a$ and $b$ are distinct ration numbers as they must be the same number. Therefore $c$ must be irrational and there exists an irrational number $c$ between an distinct rational numbers $a$ and $b$.
# 4. Use backward reasoning to show that $\sqrt{\frac{x^2+y^2}{2}}\geq\frac{x+y}{2}$ for all positive real numbers $x$ and $y$
1. $\sqrt{\frac{x^2+y^2}{2}}\geq\frac{x+y}{2}$
2. $(\sqrt{\frac{x^2+y^2}{2}})^2\geq(\frac{x+y}{2})^2$
3. $\frac{x^2+y^2}{2}\geq\frac{(x+y)^2}{4}$
4. $2(x^2+y^2)\geq(x+y)^2$
5. $2x^2+2y^2\geq x^2+2xy+y^2$
6. $x^2-2xy+y^2\geq 0$
7. $(x-y)^2\geq 0$
8. Since the left side must be positive this inequality is always true. Therefore, working backwards we can see that $\sqrt{\frac{x^2+y^2}{2}}\geq\frac{x+y}{2}$ must be true for all positive real numbers $x$ and $y$.
# 5. We say that a number, $e$, is the additive identity if $e + x = x + e = x$ for all numbers $x$. Prove that the additive identity exists and is unique.
1. $0+x=x+0=x$
2. $e_{1}+x=x+e{1}=x$
 $e_{2}+x=x+e{2}=x$
3. $e_{1}+e_{2}=e_{2}+e{1}=e_{1}$
 $e_{1}+e_{2}=e_{1}+e{1}$
 $e_{1}+e_{2}+e_{1}$
 $e_{2}=0$
4. $e_{1}+e_{2}=e_{1}+e{1}=e_{2}$
 $e_{1}+e_{2}=e_{2}$
 $e_{1}=0$
