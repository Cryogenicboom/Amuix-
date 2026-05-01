#include <stdio.h>
#include "parser.h"
#include <stdlib.h>
#include <string.h>             // to be only used for strtok()
#include <unistd.h>             // used for system calls POSIX 
#include <sys/wait.h>


void tokenize(char *user_input, char * tok_cmds[])
{
    
    int i = 0;                          // index of user_input 
    int word_idx = 0;                   // index of current word's index ( subindex )
    int token_idx = 0;                  // index to add in char * tok_cmds[]

    char buffer[100];                    // buffer to store while tracing index by index

    while(user_input[i] != '\0')
    {
        if(user_input[i] == ' ' || user_input[i] == '\t')
        {
            if(word_idx > 0)                            // if buffer is empty or filled, if filled do this
            {
                buffer[word_idx] = '\0';
                tok_cmds[token_idx] = strdup(buffer);
                word_idx = 0;
                token_idx++;
            }
            i++;
            
        }
        else if(user_input[i] == '|' || user_input[i] == '>' || user_input[i] == '<')
        {
            
            char temp[3];
            
            if(word_idx > 0)                            // if buffer is empty or filled, if filled do this
            {
                buffer[word_idx] = '\0';
                tok_cmds[token_idx] = strdup(buffer);
                word_idx = 0;
                token_idx++;
            }

            // >> or << or >& or <&
            if(user_input[i] == '>')
            {
                if(user_input[i+1] != '\0' && user_input[i+1] == '>')
                {
                    temp[0] = user_input[i];
                    temp[1] = user_input[i+1];
                    temp[2] = '\0';

                    tok_cmds[token_idx] = strdup(temp);
                    i += 2;
                    token_idx++;
                }
                else if(user_input[i+1] != '\0' && user_input[i+1] == '&')
                {
                    temp[0] = user_input[i];
                    temp[1] = user_input[i+1];
                    temp[2] = '\0';

                    tok_cmds[token_idx] = strdup(temp);
                    i += 2;
                    token_idx++;
                }
                else if(user_input[i+1] == '<')
                {
                    printf("what is this '><' command huh?");
                    return;
                }
                else
                {
                    // since strdup takes string as parameter, and '|' is a single char, conver it to string by adding \0
                    temp[0] = user_input[i];
                    temp[1] = '\0';
                    tok_cmds[token_idx] = strdup(temp);
                    i++;
                    token_idx++;            
                }

            }
            if((user_input[i] == '<' && user_input[i+1] != '\0') && (user_input[i+1] == '<' || user_input[i+1] == '&'))
            {
                printf("sorry user but this command is still in development {<<, <&, >&}");
                return;
            }
            else
            {
                // since strdup takes string as parameter, and '|' is a single char, conver it to string by adding \0
                temp[0] = user_input[i];
                temp[1] = '\0';
                tok_cmds[token_idx] = strdup(temp);
                i++;
                token_idx++;            
            }
            
        }
        else
        {
            buffer[word_idx] = user_input[i];
            word_idx++;
            i++;
        }
    }

    if(word_idx > 0)                            // if buffer is empty or filled, if filled do this
    {
        buffer[word_idx] = '\0';
        tok_cmds[token_idx] = strdup(buffer);
        word_idx = 0;
        token_idx++;
    }
    tok_cmds[token_idx] = NULL;                 // our whole tok_cmds array ends with NULL for exec (check notes)
}