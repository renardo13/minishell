## The parsing

The paring is here to help at the maximum the person who will do the executing part, it will followed the part after the tokenization, and syntax error will be treated at this moment.

#### BONUS
##### Parenthesis

The tricks to handle parenthesis is very simple. 
If you tokenize very precisely all the elements of your command line in a linked list. you can just make a counter of parenthesis regarding what will follow your parenthesis.
When you catch a valid openbracket, counter++, when you catch a clode bracket, counter--, and so on.
Open bracket should always be followed by a WORD, or another open bracket, or an operator like an AND or OR and a REDIRECTION, the next node can be NULL also.
Close bracket should aleays be followed by a REDIRECTION OUT, an AND or OR or a closed bracket.

Bash makes a distinction between arythmetique expression which is define by an expression alone, like a word alone between parenthesis and a command. An expression can also be (a + b) or (a * b), and so on.. 
In this particular context bash accept a lots of bracket like (((( a + b)))) or (((c))). This with commands is always report as a bad expression syntaxe.
