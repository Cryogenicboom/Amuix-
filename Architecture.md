This is documentation for AMUNIX specifically covers "main.c" architecture and working. For installation refer to the README.md {link readme.md} here.

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
*Yes this ASCII title was way harder than making the Shell.
<br>

<p> 
This is a hobby project of mine, i was really fascinated by Linux when playing BANDIT on my WSL. This made me learn more about Computer Networks and low level systems. Networking was not only about connecting your system to another system, but your system also uses Transmission Control Protocol (TCP) to ensure the reliablity of communication inside the system itself.
</p>
<br>

<p> 
Right now, this shell has nothing to do with Networks but it could help you too study the strucutre and how a shell operates at base level. 
</p>
<br>

> contents inside curly brackets {"xyz"} represents content from the code. Like when talking about the logic we use, we will also present a part of code to make easy reading.

### 1.0 Input and command prompt
- Take input in array `char user_input[100]` and tokenize it in another array `char *cmds[300];` with delimeter `space " " and a tab space "\t"`
- [getcwd](https://man7.org/linux/man-pages/man3/getcwd.3.html) for dynamic command prompt which returns the null terminated string containing your pathname to your current directory. 
> `
> if(getcwd(pwd, sizeof(pwd)) != NULL){
>             printf("User@system:%s $", pwd);
>         }
>  ` 

### 1.1 Tokenization and directory name with spaces
- One problem that i encountered was tokenizing a directory name which contains spaces, for example "MY FOLDER". 






