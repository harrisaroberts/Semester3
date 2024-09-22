# 1. Express each of these system specifications using predicates, quantifiers and logical connectives. Define any propositional functions that you use, and choose predicates that allow as many concepts as possible to be represented in the logic.
## (a) The e-mail address of every user can be retrieved whenever the archive contains at least one message.
$U(x):\text{x is a user}$
$M(x):\text{x is a message in the archive.}$
$R(x,y):\text{The e-mail address of user x can be retrieved whenever the archive contains at least one message y.}$
$\forall{x}\forall{y}(U(x)\land M(y)\rightarrow R(x,y))$
## (b) At least one console must be accessible during every type of fault condition.
$C(x):\text{x is a console.}$
$F(x):\text{x is a type of fault condition.}$
$A(x,y):\text{Console x is accessible during fault condition y.}$
$\forall{y}\exists{x}(C(x)\land F(y)\rightarrow A(x,y))$
## (c) There are at least two paths connecting every two distinct endpoints on the net-work.
$E(x):\text{x is an endpoint on the network.}$
$P(x,y):\text{There exists at least two paths connecting endpoints x and y.}$
$\forall{x}\forall{y}\exists{p_{1}}\exists{p_{2}}(E(x)\land E(y)\land p_{1}\neq p_{2}\land P(x,y))$
# 2. Assume the domain consists of all CS 441 students. Translate these statements using quantifiers.
## (a) Lina has best performance in the class. Let $B(x, y)$ represent the statement “$x$ performs in class better than $y$.”
$\exists{x}\forall{y}(B(x,y))$
## (b) Only one student was nominated by the class to solve a recitation problem on the board. Let $N(x, y)$ represent the statement, “$x$ nominated $y$ to solve a recitation problem on the board”
$\forall{x}\exists{y}(N(x,y))$
# 3. Let $S(x) \equiv$ “x is a CS 441 student”, $T(x) \equiv$ “x is a CS 441 TA”, $E(x, y) \equiv$ “$x$ enjoys CS 441 more than $y$”, and $A(x, y) \equiv$ “$x$ attends recitation more than $y$”. Assume the domain for each of these propositional functions consists of all the students at Pitt. Translate the logical expressions into natural English sentences.
## (a) $\exists{x}(S(x) \land \forall{y}(T(y) \rightarrow A(x, y)))$
There is one CS441 student that attends more recitations than any TA.
## (b) $\exists{x}(T(x) \land \forall{y}((T(y) \land x\neq y) \rightarrow E(x, y)))$
There is a CS441 TA who enjoys CS441 more than any other TA, besides themselves.
# 4. Use rules of inference to show that the hypotheses imply the conclusion. Consider the domains and propositions given.
## (a) Hypotheses: “It rained today”; “If it rained today, the festival will not occur”; and “If the festival does not occur, I am sad”. Conclusion: “I am sad” Let $r \equiv$ “It rained today”, $f \equiv$ “The festival occurs”, $s \equiv$ “I am sad”.
Hypothesis:
1. $r$
2. $r\rightarrow \neg{f}$
3. $\neg{f}\rightarrow s$
Conclusion:
$s$
Step 1. Modus Ponens
$r$
$r\rightarrow \neg{f}$
$\therefore \neg{f}$
Step 2. Modus Ponens
$\neg{f}$
$\neg{f}\rightarrow s$
$\therefore s$
## (b) Hypotheses: “The farm will not make profit”; “If the chickens lay eggs and the price of car insurance doesn’t increase, then the eggs are sold and the farm makes profit”; “Either the chickens lay eggs or a new car will be bought”. Conclusion: “The price of car insurance will increase or a new car will be bought” Let $c \equiv$ “The chickens lay eggs”, $e \equiv$ “Eggs will be sold”, $p \equiv$ “The farm will make profit”, $n \equiv$ “A new car will be bought”, $t \equiv$ “The price of car insurance will increase”
Hypothesis:
1. $\neg{p}$
2. $(c\land\neg{t})\rightarrow(e\land p)$
3. $c\lor n$
Conclusion:
$t\lor n$
Step 1.
$e\lor p$
$\neg{p}$
$\therefore \neg{(e\land p)}$
Step 2. Modus Tollens
$(c\land\neg{t})\rightarrow(e\land p)$
$\neg{(e\land p)}$
$\therefore \neg{(c\land\neg{t}})$
Step 3. DeMorgan's law, double negation
$\neg{(c\land\neg{t})}=\neg{c}\lor t$
Step 4. Resolution
$\neg{c}\lor t$
$c\lor n$
$\therefore t\lor v$
## (c) Hypotheses: “Somebody in CS 441 likes grilled cheese sandwiches,” “All people are either lactose intolerant or like ice cream,” and “Every person who is lactose intolerant does not like grilled cheese sandwiches.” Conclusion: “There is a person in CS 441 who likes ice cream.” Let $C(x) \equiv$ “$x$ is a student in CS 441”, $G(x) \equiv$ “$x$ likes grilled cheese sandwiches”, $L(x) \equiv$ “$x$ is lactose intolerant”, $R(x) \equiv$ “$x$ likes ice cream. Assume all domains are “all people”.
Hypothesis:
1. $\exists{x}(C(x)\land G(x))$
2. $\forall{x}(L(x)\lor R(x))$
3. $\forall{x}(L(x)\rightarrow\neg{G(x)}$
Conclusion:
$\exists{x}(C(x)\land R(x))$
Step 1. Tautology laws
$\exists{x}(C(x)\land G(x))=\exists{x}(G(x))$
Step 2. Existential instantiation
$\exists{x}(G(x))$
$\therefore G(c)\text{ for some element c}$
Step 3. Universal instantiation
$\forall{x}(L(x)\rightarrow\neg{G(x)})$
$\therefore L(c)\rightarrow \neg{G(c)}\text{ for an arbitrary c}$
Step 4. 
$L(c)\rightarrow \neg{G(c)}$
$G(c)$
$\therefore \neg{L(c)}$
Step 5. Universal instantiation
$\forall{x}(\neg{L(x)}\lor R(x))$
$\therefore \neg{L(c)} \lor R(c) \text{ for an arbitrary c}$
Step 6. Disjunctive Syllogism
$L(c)\lor R(c)$
$\neg{L(c)}$
$\therefore R(c)$
Step 7.
$R(c)$
$C(c)$
$\therefore R(c)\land C(c)$
Step 8.
$R(c)\land C(c)\text{ for some element c}$
$\therefore \exists{x}(C(x)\land R(x))$
# 5. Determine whether each of these arguments is valid. If an argument is correct, what rule of inference is being used?
## (a) If $n$ is a real number such that $n > 1$, then $n^{2} > 1$. Suppose that $n^{2} > 1$, then $n > 1$.
Valid, contropositive
## (b) If $n$ is a real number with $n > 3$, then $n^{2} > 9$. Suppose that $n^{2} \le 9$, then $n \le 3$.
Valid, contropositive
## (c) If $n$ is a real number with $n > 2, then $n^{2} > 4$. Suppose that $n \le 2$, then $n^{2} \le 4$.
Not valid

