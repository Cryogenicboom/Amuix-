#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>

void built_ins(char *parsed_cmds[])
{

    if(strcmp(parsed_cmds[0], "dbd") == 0)
        {
            printf("        DEBUG: [%s, %s, %s]\n", parsed_cmds[0], parsed_cmds[1], parsed_cmds[2]);
            if(chdir(parsed_cmds[1]) == -1)         // chdir us used to dirbadlo
            {
                perror("command directory badlo failed");
            }
            return;
        }
        else if (strcmp(parsed_cmds[0], "bahar") == 0)
        {
            exit(EXIT_SUCCESS);
        }
}
