#include <stdio.h>
#include <string.h> // to be only used for strtok()
#include <unistd.h> // used for system calls POSIX 
#include <sys/wait.h>

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


                                                                                             

void parser_for_quotes(char * cmds[], char * parsed_cmds[]){
    
    int i = 0, j = 0;

    while(cmds[i] != NULL){

        if(cmds[i][0] == '"'){
            char temp[200];
            temp[0] = '\0';
            strcat(temp, cmds[i]+1);
            while(cmds[i][strlen(cmds[i]) -1] != '"'){
                strcat(temp, " ");
                i += 1;
                strcat(temp, cmds[i]);
            }
            int temp_len = strlen(temp);
            temp[temp_len - 1] = '\0';

            parsed_cmds[j] = strdup(temp);
            j += 1;
            i += 1;
        }else{
            parsed_cmds[j] = cmds[i];
            j += 1;
            i += 1;
        }
        
        
    }
    parsed_cmds[j] = NULL;
}


int main(){
    header();
    while(1){
        char user_input[100];
        char deli[] = " \t";  // delimeter are single space or multiple spaces (tab)
        char *cmds[300]; //these commands are tokenized only
        char *parsed_cmds[100]; // these commands are parsed matlab, [ERROR 4 in diary]

        char pwd[100];
        if(getcwd(pwd, sizeof(pwd)) != NULL){
            printf("User@system:%s $", pwd);
        }
        // printf("User@system:~$");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0';




        // =================================== TOKENIZE ==================================
        char * tokenptr = strtok(user_input, deli);
        int i = 0;
        while(tokenptr != NULL){
            cmds[i] = tokenptr; // kept at top to store the first token generated outside the while loop
            // printf("arg[%d] = %s\n", i, tokenptr);
            tokenptr = strtok(NULL, deli);
            i++;
        }
        cmds[i] = NULL; // add NULL at the end of command to let know other shell (execvp) this command has terminated.


        if(cmds[0] == NULL) continue;
        else{
            parser_for_quotes(cmds, parsed_cmds);
        }

        // ========================================BUILT IN CMDS: ========================================
        if(strcmp(parsed_cmds[0], "dirbadlo") == 0){
            printf("DEBUG: [%s, %s, %s]\n", parsed_cmds[0], parsed_cmds[1], parsed_cmds[2]);
            if(chdir(parsed_cmds[1]) == -1){
                perror("dirbadlo failed");
            }
            // else if(cmds[1] == NULL){
            //     continue;
            // }
            continue;
        }

        // ================================= External Cmds: ============================================ 

        // if user empty enter then continue
        if( cmds[0] == NULL){
            continue;
        }
        
        pid_t pid = fork();

        if(pid == 0){
            execvp(parsed_cmds[0], parsed_cmds); // cmds[0] --> first line always have the command name later part contains arguments, flags etc.
            printf("\n");
        } else if(pid > 0){
            wait(NULL);
        } else{
            printf("error, fork failed");
        }
    
    }
    return 0;
}   