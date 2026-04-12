This is documentation for AMUNIX will cover Amunix commands and related. To learn about the working and internal structure you can refer to [Architecture.md](https://github.com/Cryogenicboom/Amunix/blob/main/Architecture.md), this is my personal documentation i am making as i am learning. 

## See detailed documentation here:  

[Basic Architecture](Architecture.md)
<br>

[Calc Documentation (not integrated yet)](calc/calc_doc.md)
<br>

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

## Another open shell in this open source world.

<br>

###### Yes this ASCII title was way harder than making the Shell.
<br>

<br>
<p> 
This is a hobby project of mine, i was really fascinated by Linux when playing BANDIT on my WSL. This made me learn more about Computer Networks and low level systems. Networking was not only about connecting your system to another system, but your system also uses Transmission Control Protocol (TCP) to ensure the reliablity of communication inside the system itself.
</p>
<p> 
Right now, this shell has nothing to do with Networks but it could help you too study the strucutre and how a shell operates at base level. 
</p>
<br>

## BUILT IN COMMANDS: 
| Command   | Syntax                     | Description |
|----------|----------------------------|-------------|
| dirbadlo | `dirbadlo <directory>`     | Changes the current working directory using `chdir()` system call |
| bahar    | `bahar`                    | Exits the shell program using `exit()` |

## SYSTEM COMMAND
| Feature            | Syntax Example              | Description |
|-------------------|----------------------------|-------------|
| External Commands | `ls -l`, `pwd`, `echo hi`  | Executed using `fork()` + `execvp()` :contentReference[oaicite:0]{index=0} |
| Argument Passing  | `ls -l /home`              | Arguments are passed as `char* argv[]` to `execvp()` |
| Process Handling  | (implicit)                 | Parent waits for child using `wait()` after execution |

## PARSING FEATURES
| Feature                  | Syntax Example              | Description |
|--------------------------|----------------------------|-------------|
| Pipe Separation          | `ls \| grep txt`            | Commands are split into multiple arrays using `|` :contentReference[oaicite:1]{index=1} |
| Command Count Tracking   | (internal)                 | Counts number of piped commands for execution logic |
| Argument Structuring     | (internal)                 | Stores commands as `commands[10][50]` (2D array) |

## TOKENIZATION FEATURES
| Feature                | Syntax Example              | Description |
|------------------------|----------------------------|-------------|
| Space Tokenization     | `ls -l`                    | Input split using `strtok()` with space & tab delimiters :contentReference[oaicite:2]{index=2} |
| Quote Handling         | `"My Folder"`              | Multi-word arguments handled using custom parser |
| Null Termination       | (internal)                 | Arrays end with `NULL` for compatibility with `execvp()` |

## LIMITATIONS
| Limitation            | Current Behavior |
|----------------------|------------------|
| Pipe Execution       | Pipes are parsed but NOT executed yet |
| Redirection          | Not implemented (`>`, `<`, etc.) |
| Background Process   | Not supported (`&`) |
| Error Handling       | Minimal |