#include <stdio.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>
#include "tokenizer.h"
#include "command.h"

void parse_struct(char *tokens[], Command *cmd)
{
    int c_idx = 0;              // current command index
    int argc = 0;               // argument index

    cmd->count =  1;
    cmd->inputfile = NULL;
    cmd->outputfile = NULL;

    for(int j = 0; j < 10; j++) {
    cmd->simpleCommands[j].argc = 0;
    }

    for(int i = 0; tokens[i] != NULL; i++) {

        if(strcmp(tokens[i], "|") == 0)
        {
            if(argc == 0)
            {
                printf("\nSyntax not defined: Empty command before pipe\n");
                return;
            }
            
            cmd->simpleCommands[c_idx].argv[argc] = NULL;
        
            c_idx++;
            cmd->count++;
            argc = 0;
            cmd->simpleCommands[c_idx].argc = 0;
        }
        else if(strcmp(tokens[i], ">") == 0)
        {
            if(tokens[i+1] == NULL)
            {
                printf("Syntax not defined: No output file given");
                return;
            }

            cmd->outputfile = strdup(tokens[i+1]);
            i++;                            // skip the file name
        }
        else if(strcmp(tokens[i], "<") == 0)
        {
            if(tokens[i+1] == NULL)
            {
                printf("Syntax not defined: No input file given");
                return;
            }

            cmd->inputfile = tokens[i+1];
            i++;                            // skip the file name
        }
        else
        {
            cmd->simpleCommands[c_idx].argv[argc] = tokens[i];
            argc++;
            cmd->simpleCommands[c_idx].argc = argc;

        }

    }
    if(argc == 0 && cmd->count > 1)
    {
        printf("Syntax not defined: Abondend pipe\n");
        return;
    }
    if(argc > 0)
    {
        cmd->simpleCommands[c_idx].argv[argc] = NULL;
    }

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

            while(cmds[i] != NULL && cmds[i][strlen(cmds[i]) -1] != '"')
            {          
                strcat(temp, " ");
                i += 1;

                if(cmds[i] == NULL)
                {
                    printf("Error: unmatched quote\n");
                    return;
                }

                strcat(temp, cmds[i]);
            }

            int temp_len = strlen(temp);
            if(temp_len > 0 && temp[temp_len - 1] == '"')               // replace the ending quote with null terminator.
            {
                temp[temp_len - 1] = '\0';
            }              

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

    // // DEBUG LINE 
    // printf("\nTokenized elements: ");
    // for(int a = 0; parsed_cmds[a] != NULL; a++){
    //     printf("\n%s", parsed_cmds[a]);
    // }
}


// Command strcut_cmd; 

// // ERROR 10 in diary
// void parsing_by_special_char(char * parsed_cmds[], char * commands[10][50], int *cmd_count, Command *cmd){
//     int c_idx = 0;          // command index (row)
//     int args_idx = 0;       // argument index ( column )
//     *cmd_count =1;

//     // Command strcut_cmd; 

//     for(int i =0; parsed_cmds[i] != NULL; i++)
//     {
//         if(strcmp(parsed_cmds[i], "|") == 0)
//         {
//             commands[c_idx][args_idx] = NULL;
//             c_idx += 1;
//             args_idx = 0;
//             *cmd_count += 1;
//         }
//         else if(strcmp(parsed_cmds[i], ">") == 0)
//         {
//             i++;
//             cmd->outputfile = parsed_cmds[i];
//         }
//         else if(strcmp(parsed_cmds[i], "<") == 0)
//         {
//             i++;
//             cmd->inputfile = parsed_cmds[i];
//         }

//         else{
//             commands[c_idx][args_idx++] = parsed_cmds[i];
//         }
//     }
//     commands[c_idx][args_idx] = NULL;
// }