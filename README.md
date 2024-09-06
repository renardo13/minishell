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

(Bash return generaly the unexpected token which is next the error in queston, the one which appears before).
??? So it check the error only regarding the NEXT tokeen ?????

##### --- Wildcards ---

###### Tricky cases with the wildcard :
The way that bash executes the expansion of the wildcards appears to be not lineary.

- The begenning and the end of the wildcard should automatically fit with the start and the end of the dir_name.

--> "m*.c" become "main.c"

--> "a*.c" doesn 't become "main.c"

- If there is any amount of stars between chars, it should be treated like if there was one star.

--> "m******.c" become "main.c"

It is very important to isolate the fonction that expand the variables to separate it from the parsing part. (Replace the variables by their values according to the context).
More important when you do the bonus part. 
For instance, something like 
~ "echo $USER && export USER=yop && echo $USER" 
Should expand as follow :
~ "Nameoftheuser
Yop"

You have to make the expand systematically when you call the execution of the builtins. (And in the heredoc to).

The global variable in this particular minishell project of 42 should be used to put a value that will be interpretated by a function and save the return code of the action when a signal is received.

We can say that there are 3 differents mode fir the signal.

- A signal receive in the prompt mode when no commands are being executed.
- a signal received in a "stopping" command. Like cat. 
- a signal received in a heredoc.

--> A same signal can have 3 differents actions in minishell.

I advice you to threat the redirection while treating the builtins that display something the screen (in a fd). 
By for example adding a fd parameter in your function. To echo or env or to export in the good file descriptors.

Ajouter au wilcards.
Be careful if none correspondante files to the wildcards is found. You should just considarate the name as it was writen in the prompt :

Echo *.c *llo 
~ main.c llo

###general structure of the code

In order to free in any moment and save the return code it is possible to create a mega structure that will have a pointers to all other structures and linked lists. It is also more simple to give only one or fews parameters to functions to access to others.

Personaly, I had a lots of difficulties to deal with informations that I wanted to keep untils the end of the parsing, or even during the execution. For instance the fact that my word where inbetween simple or double quotes. This will impact the fact that bash expand the variables or not and expand the wildcards or not.

I though to saves this differents information by encoding them under digits in a strings.

I had trouble to cop with spaces in the prompts. Spaces inbetween quote and spaces out : spaces that I had to keep and spaces that I had to remove after doing my split. 
I made a string to keep the information with digits of differents values.In order to restore at the end the spaces that I had. In parallel I put a random charactere that the split function will no split if it is a spaces that I had to keep. I did the same encodings systems for the expands.
To be more clear I save a sequence of 0 and 1 to know in the future if I have to expand or not as I already say before I had to separate the expand of the parsing.
Giving the fact that the command is keep in a char** every time that I change index in the tab when I expand, I also increase my index of my encoded string, and so on...
You can put all this encoded informations in a structure for instance.
Maybe there are easy way to do so, but I didn't find better way to keep informations with things that I had yo remove during the parsing to give it to the execution.


Handling error cases if quite difficults in bash. Especially to display the specific problematic token. You can treat the syntax errors by analizing what token is following the token pointed, it works good when you have to considarate errors in a relative ways. If you have to look at all the prompt in a general way maybe you can save the error token in a string in order to keep it and continue to analizing the context.

The more important for this project is the organization of the code. You should made the simplest and portables function that you can remove or add easily. You have to take into account lots of thing from the start even if you cannot know everything before starting.


## tokenizations 

You should tokenize and give a very precise label to all in your principale linked list. I used an enum structure to categorise them.

- symbols -> |, ||, && 

- sequence of characteres between quotes or simple word. "Hello my friend" "echo" "lol" that will be always a WORD enum.
Cmd[0] will always be the command.

All this differents token can have a redirection. >,>,>>,<<
That I tokenize also with enum.

By using enum structure for instance.
To tokenize redirection I put a pointers of redirection structure inside my main node. 
Every node can have a redirection.
Even node wothout anything(commands, symbols,..) can have a redirection.
It is always the first word after a redirection that is the infile. (You cannot have two or more redirections following each others.)

I only made a while loop to look for the symbols and tokenize them while treating all the characteres or sequences of characteres inbetween. If the sequence of word is betweek quote. I deserve a specific threatment to the string before puting a WORD enum label on it.
