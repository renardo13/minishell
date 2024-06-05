## The parsing

The paring is here to help at the maximum the person who will do the executing part, it will followed the part after the tokenization, and syntax error will be treated at this moment.

#### BONUS
##### --- Parenthesis ---

The tricks to handle parenthesis is very simple. 
If you tokenize very precisely all the elements of your command line in a linked list. you can just make a counter of parenthesis regarding what will follow your parenthesis.
When you catch a valid openbracket, counter++, when you catch a clode bracket, counter--, and so on.
Open bracket should always be followed by a WORD, or another open bracket, or an operator like an AND or OR and a REDIRECTION, the next node can be NULL also.
Close bracket should aleays be followed by a REDIRECTION OUT, an AND or OR or a closed bracket.

Bash makes a distinction between arythmetique expression which is define by an expression alone. This can be a word alone between at minimum two parenthesis and a command. An expression can also be ((a + b)) or ((a * b)), and so on.. 
In this particular context bash accept a lots of bracket like (((( a + b)))) or (((c))). This with a commands is always report as an error of bad expression syntaxe.

Something like that is ok ((((a))) && (((b))))
But something like this is not ((((echo a))) && (((echo b))))

##### --- Wildcards ---

###### Tricky cases with the wildcard :
The way that bash executes the expansion of the wildcards appears to be not lineary.
- If there is no characteres between the stars, AND that the char in question doesn't appears anymore, it is not valid.
  
--> "m*a*.c" doesn 't become "main.c" m*

--> "s*t*.h" become "struct.h" because it is the last "t" encountered takes into account.

- The begenning and the end of the wildcard should automatically fit with the start and the end of the dir_name.

--> "m*.c" become "main.c"

--> "a*.c" doesn 't become "main.c"

- If there is any amount of stars between chars, it should be treated like if there was one star.

--> "m******.c" become "main.c"
