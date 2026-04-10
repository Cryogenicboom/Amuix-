#include <stdio.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>

void parser_for_quotes(char * cmds[], char * parsed_cmds[])
{
    int i = 0, j = 0;    
    while(cmds[i] != NULL)
    {
        if(cmds[i][0] == '"')
        {
            char temp[200];
            temp[0] = '\0';
            strcat(temp, cmds[i]+1);
            while(cmds[i][strlen(cmds[i]) -1] != '"')   // check every 2nd letter of each index to be '"' or not.    
            {          
                strcat(temp, " ");
                i += 1;
                strcat(temp, cmds[i]);
            }

            int temp_len = strlen(temp);
            temp[temp_len - 1] = '\0';              // replace the ending quote with null terminator.

            parsed_cmds[j] = strdup(temp);
            j += 1;
            i += 1;
        }
        else
        {
            parsed_cmds[j] = cmds[i];
            j += 1;
            i += 1;
        }
    }
    parsed_cmds[j] = NULL;
}