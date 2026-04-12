#include <stdio.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>
#include "tokenizer.h"



// ERROR 10 in diary
void parsing_by_special_char(char * parsed_cmds[], char * commands[10][50], int *cmd_count){
    int c_idx = 0;          // command index (row)
    int args_idx = 0;       // argument index ( column )
    *cmd_count =1;

    for(int i =0; parsed_cmds[i] != NULL; i++)
    {
        if(strcmp(parsed_cmds[i], "|") == 0)
        {
            commands[c_idx][args_idx] = NULL;
            c_idx += 1;
            args_idx = 0;
            *cmd_count += 1;
        }
        else{
            commands[c_idx][args_idx++] = parsed_cmds[i];
        }
    }
    commands[c_idx][args_idx] = NULL;
}


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

    // DEBUG LINE 
    printf("\nTokenized elements: ");
    for(int a = 0; parsed_cmds[a] != NULL; a++){
        printf("\n%s", parsed_cmds[a]);
    }
}