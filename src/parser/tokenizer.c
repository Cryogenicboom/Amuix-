#include <stdio.h>
#include "parser.h"
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>


void tokenize(char *user_input, char * cmds[], char *parsed_cmds[]){
    char deli[] = " \t";                            // delimeter are single space or multiple spaces (tab)

    char * tokenptr = strtok(user_input, deli);
    int i = 0;
    while(tokenptr != NULL)
    {
        cmds[i] = tokenptr;                         // kept at top to store the first token generated outside the while loop
        tokenptr = strtok(NULL, deli);
        i++;
    }
    cmds[i] = NULL;                                 // add NULL at the end of command to let know other shell (execvp) this command has terminated.

    if(cmds[0] == NULL)
    {
        parsed_cmds[0] = NULL;
        return;
    } 
    
    parser_for_quotes(cmds, parsed_cmds);
    
}