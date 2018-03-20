#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void parsing_the_cmd(char* cmd, char** parameters);
int  execute_the_cmd(char** parameters);

#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMETERS 10
char *directory;
int main()
{
    char cmd[MAX_COMMAND_LENGTH + 1];
    char buff[MAX_COMMAND_LENGTH + 1];
    char* parameters[MAX_NUMBER_OF_PARAMETERS + 1];

    int cmd_count = 0;

    while(1) {
        
        char* username = getenv("USER");
        printf("%s$_shell_%d>> ", username, ++cmd_count);

    
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        
        if(cmd[strlen(cmd)-1] == '\n') {
            cmd[strlen(cmd)-1] = '\0';
        }

        
        parsing_the_cmd(cmd, parameters);

        
        if(strcmp(parameters[0], "exit") == 0) break;
        if(strcmp(parameters[0], "cd") == 0)
            {
                directory = parameters[1];
                chdir(directory);
                
                continue;
            }

        if(execute_the_cmd(parameters) == 0) break;
           
    }

    return 0;
}


void parsing_the_cmd(char* cmd, char** parameters)
{       
     while (*cmd != '\0') {       
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')

            *cmd++ = '\0';     
            *parameters++ = cmd; 

        while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') 
            cmd++;             
    } 
    *parameters = NULL;
}

int execute_the_cmd(char** parameters)
{
    // Forking the process
    pid_t p1 = fork();

    // Error
    if (p1 == -1) {
        char* error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    // Child process
    else if (p1 == 0) {
        // Execute command
        execvp(parameters[0], parameters);  

        // Error 
        char* error = strerror(errno);
        printf("shell: %s: %s\n", parameters[0], error);
        return 0;
    }

    // Parent process
    else {
        // Wait for child process to finish
        wait(NULL);
        return 1;
    }
}