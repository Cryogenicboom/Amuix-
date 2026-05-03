
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

--- 

### Another open shell in this open source world.
<br>



<p> 
This is a hobby project of mine, i was really fascinated by Linux when playing BANDIT on my WSL. This made me learn more about Computer Networks and low level systems. Networking was not only about connecting your system to another system, but your system also uses Transmission Control Protocol (TCP) to ensure the reliablity of communication inside the system itself.

AMUNIX could help you too study the strucutre and how a shell operates at base level.To learn about the working and internal structure you can refer to [Architecture.md](https://github.com/Cryogenicboom/Amunix/blob/main/ARCHITECTURE.md) 
</p>

## Run the shell
1. Fork this repo and zip file it on your system.
2. Create a new folder named "AMUNIX" and unzip the downlaoded folder here. 
2. open terminal (WIN+R --> cmd --> enter), and in terminal itself, go to the AMUNIX folder where you have extracted the shell contents.
3. Run \> `make all` 
4. then \> `make run`
5. if gnome is not downlaoded --> `sudo apt install gnome-terminal` write this in terminal and run.
6. gnome is yet to be debuged, it will be added soon.
7. `bahar` to exit

## Run Time images 
<img src="display1.png" alt="Window" width="500"/>
<br>

<img src="display2.png>" alt="Window2" width="350"/>


## See detailed documentation here:  

[Basic Architecture](ARCHITECTURE.md)
<br>

[Calc Documentation (not integrated yet)](calc/calc_doc.md)
<br>

## Command List for AMUNIX ( in development )
<br>

### BUILT IN COMMANDS: 
| Command         | Syntax                     | Description |
|-----------------|----------------------------|-------------|
| directory badlo | `dbd <directory>`          | Changes the current working directory using `chdir()` system call |
| bahar           | `bahar`                    | Exits the shell program using `exit()` |

### SYSTEM COMMAND
| Feature            | Syntax Example              | Description |
|-------------------|----------------------------|-------------|
| External Commands | `ls -l`, `pwd`, `echo hi`  | Executed using `fork()` + `execvp()`  |
| Argument Passing  | `ls -l /home`              | Arguments are passed as `char* argv[]` to `execvp()` |
| Process Handling  | (implicit)                 | Parent waits for child using `wait()` after execution |

### PARSING FEATURES
| Feature                  | Syntax Example              | Description |
|--------------------------|----------------------------|-------------|
| Pipe Separation          | `ls \| grep txt`            | Commands are split into multiple arrays using `\|` :contentReference[oaicite:1]{index=1} |
| Command Count Tracking   | (internal)                 | Counts number of piped commands for execution logic |
| Argument Structuring     | (internal)                 | Stores commands as `commands[10][50]` (2D array) |
| > (Overwrite Output)     | `ls > file.txt`            | Sends standard output to a file, creating it if it doesn't exist or overwriting existing content.|
| < (Input Redirection)    |  `wc -l << file.txt`       | Reads input for a command from a file instead of the keyboard.|

### TOKENIZATION FEATURES
| Feature                | Syntax Example              | Description |
|------------------------|----------------------------|-------------|
| Space Tokenization     | `ls -l`                    | Input split using `strtok()` with space & tab delimiters :contentReference[oaicite:2]{index=2} |
| Quote Handling         | `"My Folder"`              | Multi-word arguments handled using custom parser |
| Null Termination       | (internal)                 | Arrays end with `NULL` for compatibility with `execvp()` |

### LIMITATIONS
| Limitation            | Current Behavior |
|----------------------|------------------|
| Pipe Execution       | Pipes are parsed but NOT executed yet |
| Redirection          | Not implemented (`>`, `<`, etc.) |
| Background Process   | Not supported (`&`) |
| Error Handling       | Minimal |
