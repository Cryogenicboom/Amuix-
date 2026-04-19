This is documentation for AMUNIX covers architecture and working. For installation refer to the (README.md)[README.md] here.

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
I know there exist professional shells already. I built AMUNIX as a curiosity driven project. I was studying (OS:TEP)["https://pages.cs.wisc.edu/~remzi/OSTEP/"], this book ignited the spark in me to understand computers at fundamentals. I thought "why not learn by doing?". I hope that this shell will also help other curious minds to understand the shell development. 
</p>

# ARCHITECTURE 
Shell is divided into 3 parts: 
- [Tokenizer](#tokenizer) : splits the user input commands into array of strings. 
- [PARSING](#parsing) : gives meaning to Tokenized command. 
- [Executor](#executor) : executes the parsed command and returns output.

---

#### Initiliaz the shell with: 
- char user_intput[] : takes user input as continous one string.
- char *parsed_cmds[] : stores the tokenized commands and is used by parser.
- Struct Command , simpleCommands : stores the parsed commands and used by executor
- each command after tokenized should end with a `\0` NULL terminater, it is important because we will be using `exec()` to execute the exeternal commands. 

## Tokenizer: 

- Tokennizing a stream of text means dividing it into tokens ( like smaller strings ). But how do you actually divide ? you need a reference to divide w.r.t it, this reference is called "delimeter". 
- we are using `" "` (whitespace), `\t` (tab space) as delimeters. Whenever user enters a command `ls -a|grep file.txt` tokenizer splits this stream of text into `{"ls" "-a|grep" "file.txt"}`. But wait! `-a` and `grep` were supposed to be different commands, since they were separated by pipe `|`. 
- Your users can be hard annoying sometimes therefore they might not whitespace pipe `|`. So being a good developer you should take the responsiblity :) 
- (Piping)["https://www.geeksforgeeks.org/linux-unix/piping-in-unix-or-linux/"] : A pipe is a form of redirection that is used in Unix-like operating systems to send the output of one command/program/process to another command/program/process for further processing. The Unix systems allow the stdout of a command to be connected to the stdin of another command. You can make it do so by using the pipe character '|'. 
- to get `-a|grep` --> `"-a" "|" "grep"` i handle it manually, having a buffer array, whenever `|` is encountered, add a space before and after the pipe. and then pass it for tokenizing. 





## Parsing:





## Executor: 