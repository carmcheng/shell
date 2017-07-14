// She Sells C Shells
// CC, 2017

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {
    
    while (1) {
        
        char command[64];
        printf("shell$ ");
        fgets(command, sizeof(command), stdin);
        
        char *pos;
        if ((pos = strchr(command, '\n')) != NULL)
            *pos = '\0';
        
        char *token = strtok(command, " \t");
        
        int flag = 0;
        char *output;
        
        int n = 0;
        char *args[10];
        while (token) {
            if (strcmp(token, ">") == 0) {
                flag = 1;
                output = strtok(NULL, " \t");
                break;
            }
            args[n] = malloc(strlen(token) + 1);
            strncpy(args[n], token, strlen(token));
            n++;
            token = strtok(NULL, " \t");
        }
        args[n] = NULL;
        
        // If exit, break out
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }
        
        /***** Fork starts *****/
        int rc = fork();
        
        // CHILD
        if (rc == 0) {
            
            // Since cd does not work with execvp, use chdir
            if (strcmp(args[0], "cd") == 0) {
                int rc = chdir(args[1]);

                if (rc < 0) {
                    perror("chdir");
                    return -1;
                }
            
            // If '>' is found in the command
            } else if (flag == 1) {
                close(1);
                open(output, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                execvp(args[0], args);
                
                perror("execvp");
                return -1;
                
            // Execute command as normal
            } else {
                execvp(args[0], args);
                
                // Shouldn't run unless there is a problem
                perror("execvp");
                return -1;
            }
        }
        
        // PARENT
        else if (rc > 0) {
            int rc = wait(NULL);
            if (rc < 0) {
                perror("wait");
                return -1;
            }
            
            if (strcmp(args[0], "exit") == 0) {
                break;
            }
        }
        
        // FORK ERROR
        else {
            perror("fork");
        }
        
    } // end while loop
    
    return 0;
}
