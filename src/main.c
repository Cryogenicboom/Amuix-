#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>

#include "parser.h"
#include "tokenizer.h"
#include "command.h"
#include "executor.h"
#include "builtins.h"
#include "termios.h"

// will store the original terminal state, before i go RAW MODE 
struct termios orignal_state;

struct termios raw_state;

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
            "\nSome components may not be stable yet."
            "\nIf you run into issues or have suggestions, reporting them would be helpful by raising issues on our GITHUB repo.\n");

    printf("\nAMUIX is an another shell in this open source world. It is used to study the shell development and operating system working. Refer to this repo 'https://github.com/Cryogenicboom/User-Simulated-Virtual-OS' where we are simulating the operating system.\n\n");
}

void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orignal_state);
}

int arrow_keys(int single_char, char * cmd_history[200], int *nav_history, int *history_index, char user_input[100])
{   

    single_char = getchar();
    if(single_char == 91)
    {
        single_char = getchar();

        if(single_char == 65 && *nav_history > 0)
            {
                (*nav_history)--;
                printf("%s", cmd_history[*nav_history]);
                strcpy(user_input, cmd_history[*nav_history]);
                return strlen(user_input);
            }
        else if(single_char == 66 && *nav_history < *history_index -1)
        {
            (*nav_history)++;
            printf("%s", cmd_history[*nav_history]);
            strcpy(user_input, cmd_history[*nav_history]);
            return strlen(user_input);
        }
        else
        {
            *nav_history = *history_index;
            return 0;
        }
    }
    return -1;              // fail
}

int main()
{
    header();

    // struct termios orignal_state;
    tcgetattr(STDIN_FILENO, &orignal_state);

    // whenever exit is called switch to original  terminal state
    atexit(restore_terminal);

    // copy the original state into another struct and then use it to modify for raw.
    raw_state = orignal_state;

    raw_state.c_lflag = raw_state.c_lflag & (~ICANON);
    raw_state.c_lflag = raw_state.c_lflag & (~ECHO);

    // cooked ---> raw mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);

    // array to store command history 
    char * cmd_history[200];
    int history_index = 0;
    int nav_history = 0;

    while(1)
    {
        char user_input[100];
        char single_char;
        char *tok_cmds[300];                            //these commands are tokenized only
        char *parsed_cmds[300] = {NULL};            // these commands are parsed matlab, [ERROR 4 in diary]
        
        char pwd[100];

        if(getcwd(pwd, sizeof(pwd)) != NULL)
        {
            printf("\nUser@system:%s $", pwd);
        }

        // fgets(user_input, sizeof(user_input), stdin);
        // user_input[strcspn(user_input, "\n")] = '\0';

        // RAW MODE ======================================================================
        int i = 0;
        single_char = getchar();

        while(single_char != '\n')
        {
            printf("(%d)", single_char);
            if(single_char == 27)
            {
                i = arrow_keys(single_char, cmd_history, &nav_history, &history_index, user_input);
                
            }
            else
            {
                putchar(single_char);
                user_input[i] = single_char;
                
                fflush(stdout);                      // Notes in diary( 14 march).
                i++ ;
            }
            single_char = getchar();

        }
        putchar('\n');
        user_input[i] = '\0';

        cmd_history[history_index] = strdup(user_input);
        history_index++ ;
        nav_history = history_index;


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


        // =================================== TOKENIZE ==================================

        tokenize(user_input, tok_cmds);
        
        // =================================== Parsing ======================================

        Command cmd;
        cmd.count = 0;

        parser_for_quotes(tok_cmds, parsed_cmds);
        parse_struct(parsed_cmds, &cmd);

        // // DEBUG 
        // printf("\nStruct Debug \n");
        // for(int i = 0; i < cmd.count; i++){
        //     printf("simpleCommand %d: ", i);
        //     for(int j = 0; cmd.simpleCommands[i].argv[j] != NULL; j++){
        //         printf("[%s] ", cmd.simpleCommands[i].argv[j]);
        //     }
        //     printf("\n");
        // }

        if(parsed_cmds[0] == NULL)
        {
            continue;
        }

        // // debug
        // printf("\n      DEBUG: parsed_cmds[0] = %p\n\n", parsed_cmds[0]); // debug line to check for seg fault 


        // ========================================BUILT IN CMDS: ========================================
        if(built_ins(parsed_cmds) == 0)
        {
            continue;
        }
 
        // ================================= External Cmds: ==========================================
        
        // DEBUG COMMENT DUE TO RAW MODE ---------- UPDATE IT TOO
        
        // for(int i = 0; i < cmd.count; i++){
        //     printf("cmd[%d]: ", i);
        //     for(int j = 0; cmd.simpleCommands[i].argv[j] != NULL; j++){
        //         printf("%s ", cmd.simpleCommands[i].argv[j]);
        //     }
        //     putchar('\n');
        // }
        execute_command(&cmd, cmd.count);
         
    }
    return 0;
}   