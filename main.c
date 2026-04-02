#include <stdio.h>
#include <string.h> // to be only used for strtok()
#include <unistd.h> // used for system calls POSIX 
#include <sys/wait.h>

int main(){
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