#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <dirent.h>

const unsigned int cmd_max_len = 1024;
const unsigned int max_num_arg = 64;


void welcome_prompt();
void parse(char *input, char **argv);
void type_prompt(char *input);
void interpret_command(char **argv);


int main() {

    char input[cmd_max_len];        // The input
    char *argv[max_num_arg];        // The arguments

    int pid, status;

    welcome_prompt();

    while (1) {
        type_prompt(input);
        parse(input, argv);
        interpret_command(argv);
    }

    return 0;
}


/*
 * This functions prints the prompt message and accepts input from the user.
 */
void type_prompt(char *input) {

    printf("EEE335$ ");

    if (input != NULL) {
        int c = EOF;
        int i = 0;

        // Take in input until user hits enter or end of file is encountered.
        while ((c = getchar()) != '\n' && c != EOF) {
            input[i++] = (char) c;
        }

        input[i] = '\0';
    }
}


/*
 * This function parses the user inputs.
 */
void parse(char *input, char **argv) {

    int i = 0;
    int j = 0;
    int x = 0;
    char temp[1000];
    while(input[i] != '\0'){
        while(input[i] != '\0' && input[i] != ' '){
            temp[j++] = input[i++];
        }
        temp[j] = '\0'; //usr/bin/command
        i++;
        argv[x] = (char *)malloc(strlen(temp) + 1);
        strcpy(argv[x], temp);
        j = 0;
        x++;
        temp[0] = '\0';
    }
    argv[x] = NULL;
}


/*
 * This function interprets the parsed command that is entered by the user and
 * calls the appropriate built-in function or calls the appropriate program.
 */
void interpret_command(char **argv) {
    //BUILT IN SHELL COMMANDS
    if (strcmp(argv[0],"ls") == 0){
        //prints current directory
        DIR *p_dir;
        struct dirent *entry;
        //talk to os to tell what is the current directory
        p_dir = opendir(".");
        if (p_dir == NULL){
            perror("unable to open directory. sorry bub.");
            return;
        }

        while ((entry = readdir(p_dir)) != NULL) {
            printf("%s ", entry->d_name);
        }
        printf("\n");
        return;
    }

    else if (strcmp(argv[0],"exit") == 0){
        exit(0);
    }

    else if (strcmp(argv[0],"cd") == 0){
        char path[1024];
        strcpy(path, argv[1]);
        chdir(path);
    }

    else if (strcmp(argv[0],"pwd") == 0){
        //use the getcwd command - buffer of 1024
        char buffer[1024];
        getcwd(buffer, 1024);
        printf("Current directory: %s\n", buffer);
    }


    else{ 
       //fork
        //exec process onto the fork
        pid_t pid;
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return;
        }

        if (pid != 0){
            //Parent code
            int status;
            waitpid(pid, &status, 0);
        } else {
            //child code
            char fxnname[1024];
            strcpy(fxnname, argv[0]);
            int i = 1;
            while (argv[i] != NULL){
                strcpy(argv[i-1], argv[i++]);
            }
            execvp(fxnname, argv);
            printf("execvp failed \n");  // Only runs if execvp fails
            exit(EXIT_FAILURE);
        }
            //access file in path
            //copy that script into the process
        //if pid == pid of child, do this
        //if pid == pid of parent, do something else
    }
  return;
}




/*
 * This function prints the welcome message.
 */
void welcome_prompt() {
    int num_Padding = 41;
    int i;

    // Prints the first line of padding.
    for (i = 0; i < num_Padding; i++)
        printf("#");
    printf("\n#\tWelcome to the EEE335 shell\t#\n");
    for (i = 0; i < num_Padding; i++)
        printf("#");
    printf("\n\n");
}
