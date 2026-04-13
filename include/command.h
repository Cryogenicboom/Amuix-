#ifndef COMMAND_H
#define COMMAND_H

typedef struct{
    int argc;
    char *argv[50];
} SimpleCommand;

typedef struct{
    int count;
    SimpleCommand simpleCommands [10];  // like we did before commands[10][50]

} Command ;

#endif