There is a lot's of way to do this project. If we keep the two main phases of the project the parsing, that include the lexer and the tokenization. Basically slice the input in tokens and give it specific label. And the second phase, the execution. It is possible to make a lots of thing in the parsing. To do less at the moment of the execution, or the countrary. It depend the way you organize you. The best way I think is to do at the maximum the things at the time T. The removing of the quote, the expansion, the redirections, etc. 
the thing is, less you want to keep information in advance, less you'll have work to do. I'll develop what I'm talking about precisely after. Personaly, I wasn't enought informed about the subject before I get in. So I did a lot's of manipulation in the parsing (the part I did), but it was not necessarily. I'll explained what I did anyway.

## The parsing

The parsing is here to help at the maximum the person who will do the execution part, and all the syntax error will be treated at this moment.

#### BONUS
##### --- Parenthesis ---

The tricks to handle parenthesis is very simple. 
If you tokenize very precisely all the elements of your command line in a linked list. you can just make a counter of parenthesis that will increase or decrease only if the parenthesis is valid. The means regarding what token follow it.
When you catch a valid openbracket, counter++, when you catch a valid close bracket, counter--, and so on.
Open bracket should always be followed by a WORD, or an operator like an AND or OR and a REDIRECTION.
Close bracket should aleays be followed by a REDIRECTION OUT, an AND or OR or a closed bracket, the next node can be NULL also.

Bash makes a distinction between arythmetique expression which is define by an expression alone. This can be a word alone between at minimum two parenthesis and a command. An expression can also be ((a + b)) or ((a * b)), and so on.. 
In this particular context bash accept a lots of bracket like ((((a + b)))) or (((c))). This with a commands for instance is always report as an error of "bad expression syntaxe".
Something like that is ok ((((a))) && (((b))))
But something like this is not ((((echo a))) && (((echo b))))

##### --- Wildcards ---

###### Tricky cases with the wildcard :
The way that bash executes the expansion of the wildcards appears to be not lineary.

- The first letter and the last letter of the wildcards should necessarily fit with the first letter and the last letter of the name of the directorie(s) that has to be printed :

--> "m*.c" become "main.c"

--> "a*.c" doesn 't become "main.c"

- If there is any amount of stars between characters, it should be treated like if there was one star :

--> "m******.c" become "main.c"

Be careful if no files correspond to the wildcards you should just give back the name as it was written in the prompt :

Echo *.c *llo 
~ main.c llo

##### --- Expansion ---

###### The expansion means replacing the variables (present in the environment variables) by their values.

It is very important to separate the fonction that expand the variables from the parsing part. This is crucial when you do the bonus part.
For instance, something like :
~ "echo $USER && export USER=yop && echo $USER" 
Should expand as follow :
~ "Bernard
Yop"

You have to make the expand systematically when you call the execution of the builtins. (And in the heredoc to sometimes, when there is no quote in the name of the heredoc, the EOF).

##### --- Signals ---

You can make the signals at hte end of your minishell since you need to test it when your minishell is working properly to see if it has the right effect.
The global variable in this particular minishell project of 42 should be used to put a value that will be interpretated by a function and save the return code of the action when a signal is received.

There are 3 differents "mode" for the signal :
Same signal can have differents actions in minishell.

- a signal receive in the prompt mode when no commands are being executed.
- a signal received in a "stopping" command. Like cat. 
- a signal received in a heredoc.

##### --- Redirections ---

I advice you to threat the redirection while treating the builtins that display something the screen STDOUTFILENO or in an other fd. 
By for example adding a fd parameter in your function. To echo or env or to export in the good file descriptors.

### general structure of the code

In order to free in any moment and save the return code it is possible to create a mega structure that will have a pointers to all other structures and linked lists. It is also more simple to give only one or fews parameters to functions to access to others.

Personaly, I had a lots of difficulties to deal with informations that I wanted to keep untils after the parsing for the execution. For instance the fact that my word where inbetween simple or double quotes. This will impact the fact that bash expand the variables or not, the fact that it will expand the wildcards, etc.

I though to saves this differents information by encoding it withs digits(code) in differents strings.

To beggin, I had troubles to cop with spaces in the prompts because I use split function to save the commands with the options, something that you don't have to do necessarily, again it depends of your proper implementation of minishell. To deal with spaces that I had to keep and spaces that I had to remove to do my split I made a string to keep the information with digits of differents values, in order to restore after the spaces that I had before. In parallel I put a random charactere that the split function will "ignore" if it is a spaces that I had to keep in the future. I did the same encodings systems for the expands, with 0 if I do not, and 1 if I had to replace the name by the value.
To be more clear I save a sequence of 0 and 1 to know in the future if I have to expand or not as I already say before I had to separate the expand of the parsing.
You can put all this encoded informations in a structure deserve to that for instance.
There are other way to do so, but I didn't find better way to keep informations that I had to remove during the parsing and but keep accessible under another form during the differents phases of the execution.

##### Syntax error

It can be nasty to display the specific error token. You can treat the syntax errors by analizing what token is following the token pointed, it works well when you have to considerate errors in a relative ways. If you have to look at all the token in your linked list in a general way maybe you can save the error token in a string in order to keep it and continue to analizing the context, maybe this kind of error are more important compared to others. Like odd quote for exemple, or not closed parenthesis.

The more important for this project is the organization of the code. You should made the simplest and portables function that you can remove or add easily. You have to take into account lots of thing from the start.

##### tokenizations 

You should tokenize and give a very precise label to all in your elements in your linked list. I used an enum structure to categorise them, it is simply digits aimed to identify a token. I choose to determine 2 differents epressions that has to be treated separately.

- symbols -> |, ||, && 

- sequence of characteres between quotes or simple word. "Hello my friend" "echo" "lol" will always be a WORD enum.
Cmd[0] will always be the command.

All this differents token can have a redirection. >,>,>>,<<. So I decided to make a linked list inside my principal linked list. A pointer of redir_struct inside my node cmd_struct.
I tokenize the redir also with enum structure. Even node without anything(commands, symbols,..) can have a redirection.
It is always the first word after a redirection that is the infile or the outfile. (You cannot have two or more redirections following each others.)

In conclusion, I only made a while loop to look for the symbols and tokenize them while treating all the characteres or sequences of characteres inbetween. If the sequence of word is between quote, I treated it like a bloc and I deserve a specific threatment to the string before puting a WORD enum label on it.
