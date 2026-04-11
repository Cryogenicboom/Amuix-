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
<br>

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







