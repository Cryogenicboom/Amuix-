#include <stdio.h>
#include <string.h> // to be only used for strtok()
#include <unistd.h> // used for system calls POSIX 
#include <sys/wait.h>

void header(){
    printf(
    "      .o.       ooo        ooooo ooooo     ooo ooooo ooooooo  ooooo \n"
    "     .888.      `88.       .888' `888'     `8' `888'  `8888    d8'  \n"
    "    .8\"888.     888b     d'888   888       8   888     Y888..8P    \n"
    "   .8' `888.     8 Y88. .P  888   888       8   888      `8888'     \n"
    "  .88ooo8888.    8  `888'   888   888       8   888     .8PY888.    \n"
    " .8'     `888.   8    Y     888   `88.    .8'   888    d8'  `888b   \n"
    "o88o     o8888o o8o        o888o    `YbodP'    o888o o888o  o88888o \n"
    "\n"
    "\n"
    "\n"
    );

    printf("This project is being actively developed as a learning and building exercise."
            "\nSome components may not be fully stable yet."
            "\nIf you run into issues or have suggestions, reporting them would be helpful.\n");

    printf("\nAMUIX is an another shell in this open source world. It is used to study the shell development and operating system working. Refer to this repo 'https://github.com/Cryogenicboom/User-Simulated-Virtual-OS' where we are simulating the operating system.\n\n");
}

int main(){
    header();
    while(1){
        char user_input[100];
        char deli[] = " \t";  // delimeter are single space or multiple spaces (tab)
        char *cmds[30];
        printf("User@system:~$");
        fgets(user_input, sizeof(user_input), stdin);
        
        user_input[strcspn(user_input, "\n")] = '\0';

        char * tokenptr = strtok(user_input, deli);
        int i = 0;
        while(tokenptr != NULL){
            cmds[i] = tokenptr; // kept at top to store the first token generated outside the while loop
            // printf("arg[%d] = %s\n", i, tokenptr);
            tokenptr = strtok(NULL, deli);
            i++;
        }
        cmds[i] = NULL; // add NULL at the end of command to let know other shell (execvp) this command has terminated.

        // if user empty enter then continue
        if( cmds[0] == NULL){
            continue;
        }
        
        pid_t pid = fork();

        if(pid == 0){
            execvp(cmds[0], cmds); // cmds[0] --> first line always have the command name later part contains arguments, flags etc.
            printf("\n");
        } else if(pid > 0){
            wait(NULL);
        } else{
            printf("error, fork failed");
        }
    
    }
    return 0;
}   