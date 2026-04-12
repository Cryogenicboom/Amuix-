This is documentation for AMUNIX specifically covers "main.c" architecture and working. For installation refer to the README.md {will update soon} here.

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

###### Yes this ASCII title was way harder than making the Shell.

<br> 

## Another open shell in this open source world.

<br>
<br>

<p> 
This is a hobby project of mine, i was really fascinated by Linux when playing BANDIT on my WSL. This made me learn more about Computer Networks and low level systems. Networking was not only about connecting your system to another system, but your system also uses Transmission Control Protocol (TCP) to ensure the reliablity of communication inside the system itself.
</p>
<p> 
Right now, this shell has nothing to do with Networks but it could help you too study the strucutre and how a shell operates at base level. 
</p>
<br>


### 1.0 Input and command prompt
- Take input in array `char user_input[100]` and tokenize it in another array `char *cmds[300];` with delimeter `space " " and a tab space "\t"`
- [getcwd](https://man7.org/linux/man-pages/man3/getcwd.3.html) for dynamic command prompt which returns the null terminated string containing your pathname to your current directory. 
> ```
> if(getcwd(pwd, sizeof(pwd)) != NULL){
>             printf("User@system:%s $", pwd);
>         }
> ```

---

### 1.1 Tokenization and directory name with spaces
- One problem that i encountered was tokenizing a directory name which contains spaces, for example "MY FOLDER".
- using [strtok()](https://cplusplus.com/reference/cstring/strtok/) tokenizes the `user_input` in array of strings, replacing every delimeter by a null terminater. Therefore we establish a rule in our shell that if any single entity contains a whitespace in them, it has to be enclosed in double quotes.
- so when we encounter a double quotes, we parse it manually, starts concatinating strings in a buffer array until end of quotes, and then duplicate the buffer_array_element into `char * parsed_cmds[100]`
> ```
> if(cmds[i][0] == '"'){ 
>    char temp[200];
>    temp[0] = '\0';
>    strcat(temp, cmds[i]+1);
>      while(cmds[i][strlen(cmds[i]) -1] != '"'){
>        strcat(temp, " ");
>        i += 1;
>        strcat(temp, cmds[i]);
>      }
>    int temp_len = strlen(temp);
>    temp[temp_len - 1] = '\0';
>    parsed_cmds[j] = strdup(temp);
>    j += 1;
>    i += 1;
> }
> ```

### 1.2 `ls -l|grep file.txt` 
- Above command is what we will be using to study further development. so let's study what this line of command means: 
> - list the detailed information (meta data) about all files in the directory and pass it as input to `grep` command which will write it in the file.txt

- The above command is tokenized as : [ "ls", "-l|grep", "file.txt", "NULL" ].
- Hey wait, `-l` and `grep` are different commands, so we need to tokenize it differently. Also we need to consider pipe `|` as a token too, since it holds a meaning. 
- User should have entered `ls -l | grep file.txt` (with spaces), you should know your users, they like things to be easy, and that's why you as a programmer have to take care of every case (its a pain ikr).
- **NOTE:** pipe is to be considered as a char '|' ✅ not string "|" ❌.


### 2.0 Parsing and Pipe
- After tokenization, we now have a clean array of strings:
- > `parsed_cmds[] = {"ls", "-l", "|", "grep", "txt", NULL}`

- Now the goal of parsing is not just splitting strings, but understanding structure.
- A shell does not see this as one command, it sees:

> Command 0 → ls -l  
> Command 1 → grep txt

- This idea comes from the concept of a Command Table, where the command is split into multiple commands keeping '|' as delimeter. That's why space before and after '|' is necessary. 

### 2.1 Why Parsing is Needed

- Tokenization only splits your user_input string like it asks --> “What are the words?”

- Parsing gives meaning by spliting them further into logical manner, it asks --> “How are these words grouped and executed?”

- > Without parsing: ls -l | grep txt → treated as one command 
  > With parsing, Split into multiple commands & execute them properly 

### 2.2 Pre-processing Trick 

- Before tokenization, we do:

>  if(user_input[user_idx] == '|')
>  {
>      buffer[buffer_idx] = ' ';
>      buffer[buffer_idx + 1] = '|';
>      buffer[buffer_idx + 2] = ' ';
>  }

This ensures that `ls|grep` --> `ls | grep`, Without this strtok() would treat ls|grep as one token.


### 2.3 Parsing function: 

- We treat | as a separator between commands, not just another token.
- Function in parser.c does the work : 
  > void parsing_by_special_char(char * parsed_cmds[], char * commands[10][50], int *cmd_count)
- What it does is iterates over parsed_cmds[] and when it sees "|", it ends current command (NULL) and moves to next row.
- Otherwise it adds token to current command.
- After parsing:
  > commands[0] --> {"ls", "-l", NULL}
  > commands[1] --> {"grep", "txt", NULL}

### 2.4 NULL TERMINATER

- you must be seeing or if not then let me make it visible for you that every command we store ends with `NULL` WHY?
- > commands[c_idx][args_idx] = NULL;
- Because execvp() requires NULL-terminated argument lists. If we forget this we won’t get compile errors but get silent undefined crashes. 
- Also it allows us to parse / trace strings of commands easily by just doing `if != NULL`.
