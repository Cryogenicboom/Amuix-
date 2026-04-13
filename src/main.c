#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>

#include "parser.h"
#include "tokenizer.h"
#include "command.h"

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
        char *cmds[300];                            //these commands are tokenized only
        char *parsed_cmds[300] = {NULL};            // these commands are parsed matlab, [ERROR 4 in diary]
        char *commands [10][50];                     // final parsing commands (after pipe)
        
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

        tokenize(user_input, cmds, parsed_cmds);
        
        // =================================== Parsing ======================================

        int cmd_count = 0;
        parsing_by_special_char(parsed_cmds, commands, &cmd_count);         // using & with cmd_count because cmd_count is earlier defined as int only, unlike other array parameters which were defined with pointer state 

        // using struct; copying my commands to struct data type 
        Command cmd;
        cmd.count = cmd_count;

        for(int i = 0; i < cmd_count; i++){
            cmd.simpleCommands[i].argc = 0;

            for(int j = 0; commands[i][j] != NULL; j++){
                cmd.simpleCommands[i].argv[j] = commands[i][j];
                cmd.simpleCommands[i].argc++;
            }
            cmd.simpleCommands[i].argv[cmd.simpleCommands[i].argc] = NULL;
        }

        // DEBUG 
        printf("\nStruct Debug \n");
        for(int i = 0; i < cmd.count; i++){
            printf("simpleCommand %d: ", i);
            for(int j = 0; cmd.simpleCommands[i].argv[j] != NULL; j++){
                printf("%s", cmd.simpleCommands[i].argv[j]);
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
        if(strcmp(parsed_cmds[0], "dirbadlo") == 0)
        {
            printf("        DEBUG: [%s, %s, %s]\n", parsed_cmds[0], parsed_cmds[1], parsed_cmds[2]);
            if(chdir(parsed_cmds[1]) == -1)         // chdir us used to dirbadlo
            {
                perror("dirbadlo failed");
            }
            continue;
        }
        else if (strcmp(parsed_cmds[0], "bahar") == 0)
        {
            exit(EXIT_SUCCESS);
        }


        // ================================= External Cmds: ==========================================
        
        pid_t pid = fork();

        if(pid == 0)
        {
            execvp(cmd.simpleCommands[0].argv[0], cmd.simpleCommands[0].argv);            // cmds[0] --> first line always have the command name later part contains arguments, flags etc.
            printf("\n");
        } 
        else if(pid > 0)
        {
            wait(NULL);
        } 
        else
        {
            printf("error, fork failed");
        }
    }
    return 0;
}   