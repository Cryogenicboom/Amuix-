#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>

#include "parser.h"
#include "tokenizer.h"
#include "command.h"
#include "executor.h"

void header(){  

    printf(
    "       o           oooo     oooo      ooooo  oooo      oooo   oooo      ooooo      ooooo  oooo       \n"
    "      888           8888o   888        888    88        8888o  88        888         888  88         \n"
    "     8  88          88 888o8 88        888    88        88 888o88        888           888           \n"
    "    8oooo88         88  888  88        888    88        88   8888        888           888       \n"
    "   88     88        88   88  88        888    88        88    888        888         888  88         \n"
    "  88       88       88       88        888    88        88     88        888        888    88        \n"
    " o88o    o888o     o88o     o88o       o888oo88o       o88o    o88o     o888o     o88o    o888o      \n"
    "                                                                                                  \n"
    );

    printf("\nThis project is being actively developed as a learning and building exercise."
            "\nSome components may not be fully stable yet."
            "\nIf you run into issues or have suggestions, reporting them would be helpful.\n");

    printf("\nAMUIX is an another shell in this open source world. It is used to study the shell development and operating system working. Refer to this repo 'https://github.com/Cryogenicboom/User-Simulated-Virtual-OS' where we are simulating the operating system.\n\n");
}


int main()
{
    header();
    while(1)
    {
        char user_input[100];
        char *tok_cmds[300];                            //these commands are tokenized only
        char *parsed_cmds[300] = {NULL};            // these commands are parsed matlab, [ERROR 4 in diary]
        
        char pwd[100];

        if(getcwd(pwd, sizeof(pwd)) != NULL)
        {
            printf("User@system:%s $", pwd);
        }

        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0';

        // debug
        printf("        before : %s", user_input);

        // add spaces before and after pipe |
        int buffer_idx = 0;
        char buffer[300];
        for(int user_idx =0; user_input[user_idx] != '\0' ; user_idx++)
        {
            if(user_input[user_idx] == '|')
            {
                
                buffer[buffer_idx] = ' ' ;
                buffer[buffer_idx + 1] = '|';
                buffer[buffer_idx + 2] = ' ';
                buffer_idx += 3;

            }
            else
            {
                buffer[buffer_idx] = user_input[user_idx];
                buffer_idx += 1;
            }
        }

        buffer[buffer_idx] = '\0';
        strcpy(user_input, buffer);

        // debug
        printf("\n      after : %s", user_input);

        // =================================== TOKENIZE ==================================

        tokenize(user_input, tok_cmds);
        
        // =================================== Parsing ======================================

        Command cmd;
        cmd.count = 0;

        parser_for_quotes(tok_cmds, parsed_cmds);
        parse_struct(parsed_cmds, &cmd);

        // DEBUG 
        printf("\nStruct Debug \n");
        for(int i = 0; i < cmd.count; i++){
            printf("simpleCommand %d: ", i);
            for(int j = 0; cmd.simpleCommands[i].argv[j] != NULL; j++){
                printf("[%s] ", cmd.simpleCommands[i].argv[j]);
            }
            printf("\n");
        }

        if(parsed_cmds[0] == NULL)
        {
            continue;
        }

        // debug
        printf("\n      DEBUG: parsed_cmds[0] = %p\n\n", parsed_cmds[0]); // debug line to check for seg fault 


        // ========================================BUILT IN CMDS: ========================================
        if(strcmp(parsed_cmds[0], "dbd") == 0)
        {
            printf("        DEBUG: [%s, %s, %s]\n", parsed_cmds[0], parsed_cmds[1], parsed_cmds[2]);
            if(chdir(parsed_cmds[1]) == -1)         // chdir us used to dirbadlo
            {
                perror("command directory badlo failed");
            }
            continue;
        }
        else if (strcmp(parsed_cmds[0], "bahar") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        

        // ================================= External Cmds: ==========================================
        printf("\nDEBUG:\n");
        printf("inputFile: %s\n", cmd.inputfile ? cmd.inputfile : "NULL");
        printf("outputFile: %s\n", cmd.outputfile ? cmd.outputfile : "NULL");

        for(int i = 0; i < cmd.count; i++){
            printf("cmd[%d]: ", i);
            for(int j = 0; cmd.simpleCommands[i].argv[j] != NULL; j++){
                printf("%s ", cmd.simpleCommands[i].argv[j]);
            }
            printf("\n");
        }
        execute_command(&cmd, cmd.count);
         
    }
    return 0;
}   