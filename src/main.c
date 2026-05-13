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

struct cmd_history
    {
        char *h_cmds;
        struct cmd_history *next;
        struct cmd_history *previous;
    };
    
// history command head
struct cmd_history *head = NULL;
struct cmd_history *tail = NULL;

    

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

    printf("\n\x1b[31mAMUIX is an another shell in this open source world. It is used to study the shell development and operating system working. Refer to this repo 'https://github.com/Cryogenicboom/User-Simulated-Virtual-OS' where we are simulating the operating system.\x1b[0m\n\n");
}

void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orignal_state);
    free(tail);
    free(head);
}

int arrow_keys(char single_char, char user_input[100], struct cmd_history *tail, struct cmd_history *head, int i, struct cmd_history **navptr)
{   
    single_char = getchar();

    if(single_char == 91)
    {   
        if(*navptr == NULL)
        {
            return 0;
        }

        single_char = getchar();
        
        for(int j = 0; j < i; j++)
        {
            printf("\b \b");
        }

        if(single_char == 65 && *navptr != head)
            {   
                *navptr = (*navptr)->previous;
                printf("%s", (*navptr)->h_cmds);
                strcpy(user_input, (*navptr)->h_cmds);
                return strlen(user_input);
            }
        else if(single_char == 66)
        {
            if(*navptr != tail)
            {
                *navptr = (*navptr)->next;
                printf("%s", (*navptr)->h_cmds);
                strcpy(user_input, (*navptr)->h_cmds);
                return strlen(user_input);
            }
            else 
            {   
                strcpy(user_input, "\0");
                return 0;
            }
        }
        else
        {
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

    // whenever exit is called switch to og terminal state
    atexit(restore_terminal);

    // copy the original state into another struct and then use it to modify for raw.
    raw_state = orignal_state;

    raw_state.c_lflag = raw_state.c_lflag & (~ICANON);
    raw_state.c_lflag = raw_state.c_lflag & (~ECHO);

    // cooked ---> raw mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_state);

    struct cmd_history *navptr = tail;                // pointer to navigate the history array

    // tail = head;

    while(1)
    {
        char user_input[100];
        char single_char;
        char *tok_cmds[300];                            //these commands are tokenized only
        char *parsed_cmds[300] = {NULL};               // these commands are parsed matlab, [ERROR 4 in diary]
        
        char pwd[100];

        if(getcwd(pwd, sizeof(pwd)) != NULL)
        {
            printf("\n\x1b[32mUser@system:%s $\x1b[0m", pwd);
        }

        // fgets(user_input, sizeof(user_input), stdin);
        // user_input[strcspn(user_input, "\n")] = '\0';

        // RAW MODE ======================================================================
        int i = 0;
        single_char = getchar();

        while(single_char != '\n')
        {
            // printf("(%d)", single_char);
            if(single_char == 27)
            {   
                int temp = arrow_keys(single_char, user_input, tail, head, i, &navptr);
                if(temp >= 0)               
                {
                    i = temp;
                }
            }
            else if(single_char == 127 && i > 0)
            {
                printf("\b \b");
                i--;
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

        if(strlen(user_input) != 0)
        {
            // if block only runs at start, when tail = head
            if(tail == NULL)
            {
                tail = (struct cmd_history *) malloc(sizeof(struct cmd_history));   // first node 
                tail->h_cmds = strdup(user_input);
                tail->previous = NULL;
                head = tail;                      // head points to first node 
                tail->next = NULL;
            }
            else
            {   
                // next node create
                struct cmd_history *temp = NULL;
                temp = (struct cmd_history *) malloc(sizeof(struct cmd_history));
                temp->previous = tail;                  // store the prev. node address to new node's prev ptr
                tail->next = temp;                      // old node store the new node address
                tail = temp;                            // move tail to new node
                tail->h_cmds = strdup(user_input);      // store the new command in new node
                tail->next = NULL;   

                // // ERROR 22 in diary ( 22 march )
                // temp = (struct cmd_history *)malloc(sizeof(struct cmd_history));
                // tail->next = temp;    
                // temp->previous = tail;
                // tail = temp;

            }
            navptr = tail;
        }

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
        // printf("\n      DEBUG: parsed_cmds[0] = %p\n\n", parsed_cmds[0]); //check for seg fault 


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