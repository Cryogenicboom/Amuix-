This is documentation for AMUNIX covers architecture and working. For installation refer to the [README.md](README.md) here.

```
        .o.       ooo        ooooo ooooo     ooo  oooo        oooo  ooooo ooooooo  ooooo
       .888.      `88.       .888' `888'     `8'   `88         88   `888`  `8888    d8'
      .8\"888.     888b     d'888   888       8     888b       88    888     Y888..8P
     .8' `888.     8 Y88. .P  888   888       8     8 Y88.     88    888      `8888'
    .88ooo8888.    8  `888'   888   888       8     8  `888'   88    888     .8PY888.
   .8'     `888.   8    Y     888   `88.    .8'     8    Y888 .88    888     d8'  `888b
  o88o     o8888o o8o        o888o    `YbodP'      o8o      88888o  o888o oo888o  o88888o 
```
<br>

#### Yes this ASCII title was way harder than making the Shell.
<br> 

## What is Shell?
<p>
Shell is a CLI (Command Line Interface) that acts as an interface for an Operating System. Shell enables user to interact with kernal. it provides user with commands, execute programmes for them and manages Input and Output functionality. 
</p>

## Why AMUNIX? 
<p>
I know there exist professional shells already. I built AMUNIX as a curiosity driven project. I was studying [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/) , this book ignited the spark in me to understand computers at fundamentals. I thought "why not learn by doing?". I hope that this shell will also help other curious minds to understand the shell development. 
</p>

# ARCHITECTURE 
Shell is divided into 3 parts: 
- [Tokenizer](#10-tokenizer) : splits the user input commands into array of strings. 
- [PARSING](#20-parsing) : gives meaning to Tokenized command. 
- [Executor](#executor) : executes the parsed command and returns output.

---

#### Initiliaz the shell with: 
- `char user_intput[]` : takes user input as continous one string.
- `char *parsed_cmds[]` : stores the tokenized commands and is used by parser.
- Struct Command , simpleCommands : stores the parsed commands and used by executor
- each command after tokenized should end with a `\0` NULL terminater, it is important because we will be using `exec()` to execute the exeternal commands. 

## 1.0 Tokenizer: 
Converts raw user input (character stream) into a sequence of tokens which can be used for parsing and make meaningful commands out of it.

#### 1.1 Input:

- user_input: mutable C string read from stdin (newline removed). input is preprocessed before tokenizer to add the NULL terminator.
- __FLAW__ : Assumes preprocessing step (main.c) inserts spaces around "|" before tokenization. 
- Maximum input size bounded by fixed buffer (100 chars in current implementation)

#### 1.2 Output

- cmds[]: intermediate token array produced with delimeter 'whitespaces' or 'tab'.
- parsed_cmds[]: final token array after quote handling. Example : `dirbadlo "directory name with spaces"`.
- Both arrays are NULL terminated to satisfy execvp() argument requirements.


#### 1.3 Tokenization Rules

- Delimiters: space ' ' and tab '\t'
- Consecutive delimiters are treated as a single separator (strtok behavior)
- Quoted substrings ("...") are merged into a single token
- Special symbols (|, <, >) are not tokenized by tokenizer and rely on preprocessing or later parsing stages.

#### 1.4 Processing Flow

- Input string is split using strtok() into cmds[]
- If input is empty, return NULL output
- parser_for_quotes() merges quoted tokens into parsed_cmds[]
- Final token array is prepared for parsing stage

#### 1.5 Limitations
- strtok() is a C function defined in `<string.h>` header. It is used to tokenized the input, rather than dealing it with manually. 
- Relies on manual preprocessing for operators like | (not lexical)
- Does not support escaped quotes (\") or nested quoting
- Operators (>>, >&, etc.) are not handled at tokenizer level ( still in development )
- Mixed memory ownership: some tokens point into input buffer, others are heap-allocated (strdup)

## 2.0 Parsing:
Parser is another tool that gives meaning to our commands, it breaks our commands in such a way that a computer can understand. It takes tokenized commands and gives out a command table ( struct ). This is where we will also look for errors and handle wrong syntax entered by user. 
For arguments like `"Directory name with spaces"` they are divided into tokens, we will handle double quotes to ensure string inside double quotes be a token. Once tokens are normalized we will proceed to forming command table.

### 2.1 parsing with quotes 
- `parser_for_quotes()` processes the token list to handle quoted strings. Since the tokenizer is splitting the input based on spaces and special characters, arguments enclosed in quotes (ex. "hello world") are incorrectly split into multiple tokens. This function reconstructs them into a single argument by merging tokens until a closing quote is found.
- This ensure correct syntax called semantic correctness of arguments. 

## 2.2 Output: 
> - an array of SimpleCommands (pipeline stages),
> - argument vectors (argv) for each command,
> - input/output redirection information.

### 2.3 Command table
- `parse_struct()` performs structural parsing. It iterates over the tokens and builds the Command structure by organizing tokens into multiple `SimpleCommand` entries. 
- Each `SimpleCommand` represents a command in a pipeline, and arguments are stored in an `argv` array format compatible with execvp(). 
- Pipes (|) are used to separate commands, while redirection operators (<, >) update the input and output file fields of the Command structure. see the `command.h` file for the defination of `Command` struct.

## 2.4 What we are doing ? 
- parser enforces basic syntax rules during, such as preventing empty commands before pipes and ensuring that redirection operators are followed by valid filenames. If invalid syntax is detected, the parser reports an error and stops further processing.
- Finally, the output of the parser is a fully populated Command structure as mentioned in [Output](#22-output).
- This structure is then passed to the executor, which uses it to create processes, set up pipes, and perform execution.


## Executor: 