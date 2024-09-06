# Project Overview

There are lots of ways to do this project. If we keep the two main phases of the project: the parsing, which includes the lexer and the tokenization (basically slicing the input into tokens and giving them specific labels), and the second phase: the execution. It is possible to do lots of things in the parsing to reduce the work at the time of execution, or the contrary. It depends on how you organize it. The best way, I think, is to do as much as possible at the time T: removing quotes, handling expansions, redirections, etc.

The thing is, the less information you keep in advance, the less work you'll have to do. I'll explain exactly what I mean later. Personally, I wasn't informed enough about the subject before I started, so I did a lot of manipulation in the parsing (the part I did), but it wasn't necessarily the best approach. I'll explain what I did anyway.

## Parsing

The parsing is here to help the person doing the execution as much as possible, and all the syntax errors will be treated at this stage.

### BONUS

#### Parentheses

The trick to handling parentheses is very simple. If you tokenize every element of your command line into a linked list, you can create a counter for parentheses that will increase or decrease only when the parentheses are valid, depending on what token follows them. 

- When you catch a valid open bracket, `counter++`. 
- When you catch a valid close bracket, `counter--`. 
- Open brackets should always be followed by a WORD, an operator like AND or OR, or a REDIRECTION. 
- Close brackets should always be followed by a REDIRECTION OUT, an AND or OR, or a closing bracket. The next node can also be `NULL`.

Bash distinguishes between arithmetic expressions, which are defined as expressions alone. This can be a word alone between at least two parentheses or a command. An expression can also be `((a + b))` or `((a * b))`, and so on. In this specific context, Bash accepts many brackets like `((((a + b))))` or `(((c)))`. However, with commands, Bash will report a "bad expression syntax" error, for example:
- `((((a))) && (((b))))` is valid.
- `((((echo a))) && (((echo b))))` is invalid.

#### Wildcards

##### Tricky cases with wildcards:

The way Bash executes wildcard expansion does not seem linear.

- The first and last letters of the wildcard must necessarily match the first and last letters of the directory names to be printed:
  - `"m*.c"` becomes `main.c`
  - `"a*.c"` does **not** become `main.c`

- If there is any number of asterisks between characters, it should be treated as if there was only one asterisk:
  - `"m******.c"` becomes `main.c`

Be careful: if no files match the wildcards, you should just return the name as it was written in the prompt. For example:
  - `echo *.c *llo` results in: `main.c *llo`

#### Expansion

Expansion means replacing the variables (present in the environment variables) by their values.

It is very important to separate the function that expands the variables from the parsing part. This is crucial when you do the bonus part. For example:
- `echo $USER && export USER=yop && echo $USER` should expand as:
- "Bernard
Yop"


You have to perform the expansion systematically when you call the execution of the builtins (and sometimes in heredoc, when there is no quote in the name of the heredoc, the EOF).

#### Signals

You can handle signals at the end of your minishell since you need to test it when your minishell is working properly to see if it has the right effect. The global variable in this particular 42 minishell project should be used to store a value that will be interpreted by a function and store the return code of the action when a signal is received.

There are 3 different "modes" for the signal:

- A signal received in prompt mode when no commands are being executed.
- A signal received in a "stopping" command, like `cat`.
- A signal received in a heredoc.

#### Redirections

I advise you to handle redirections while treating builtins that display something to the screen (`STDOUT_FILENO`) or another file descriptor. For example, by adding a file descriptor parameter to your function, such as for `echo`, `env`, or `export`, to handle output in the correct file descriptor.

### General Structure of the Code

To free memory at any time and store the return code, it is possible to create a mega structure that will have pointers to all other structures and linked lists. It is also simpler to pass only one or a few parameters to functions to access the others.

Personally, I had a lot of difficulties managing information that I wanted to keep after parsing for execution. For example, whether my word was between single or double quotes impacts whether Bash expands variables or not, whether it expands wildcards, etc.

I thought of saving this information using digits (codes) in different strings.

At first, I had trouble dealing with spaces in the prompt because I used a split function to save the commands with options, which is something you don't necessarily need to do. Again, it depends on your implementation of minishell. To handle spaces that needed to be kept and spaces that needed to be removed for splitting, I created a string to store this information using digits of different values, allowing me to restore the spaces afterward. In parallel, I inserted a random character that the split function would "ignore" if it was a space that needed to be kept. I applied the same encoding system for expansions, using `0` for no expansion and `1` for replacing the name with the value.

To clarify, I stored a sequence of `0`s and `1`s to indicate whether I needed to expand or not. As I mentioned earlier, I separated expansion from parsing. You can store all this encoded information in a structure dedicated to this purpose. There are other ways to do this, but I didn’t find a better way to retain the information I had to remove during parsing while keeping it accessible in a different form during execution.

#### Syntax Errors

It can be tricky to display the specific error token. You can handle syntax errors by analyzing which token follows the current one, which works well when you need to handle errors in a relative manner. If you need to scan all the tokens in your linked list more generally, you can store the error token in a string to keep it while continuing to analyze the context. These types of errors may be more significant compared to others, such as mismatched quotes or unclosed parentheses.

### Tokenization

You should tokenize and give a precise label to all the elements in your linked list. I used an enum structure to categorize them—simply digits aimed at identifying a token. I decided to handle two distinct types of expressions separately:

- **Symbols** like `|`, `||`, `&&`
- **Sequences of characters between quotes or a simple word**: `"Hello my friend"`, `"echo"`, and `"lol"` will always be labeled as WORD enums. `Cmd[0]` will always be the command.

All these different tokens can have a redirection: `>`, `>>, <<`. So I created a linked list within my main linked list, with a pointer to a `redir_struct` inside my `cmd_struct` node. I also tokenized redirections with an enum structure. Even nodes without anything (commands, symbols, etc.) can have a redirection. The first word after a redirection will always be the infile or outfile (you cannot have two or more consecutive redirections).

In conclusion, I simply used a while loop to look for symbols and tokenize them while processing characters or sequences of characters in between. If the sequence of words was between quotes, I treated it as a block and applied specific treatment to the string before labeling it with a WORD enum.

