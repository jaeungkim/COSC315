// closh.c - COSC 315, Winter 2020
// ASSIGNMENT 1
// Jae Ung Kim (37007135)
// Deokhee Yoon (67262030)
// Joseph Choi (58178138)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;wcl
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// main method - program entry point
int main() {
    char cmd[81]; // array of chars (a string)
    char* cmdTokens[20]; // array of strings
    int count; // number of times to execute command
    int parallel; // whether to run in parallel or sequentially
    int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)
    
    while (TRUE) { // main shell input loop
        
        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n') continue;
        readCmdTokens(cmd, cmdTokens);
        do {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);
        
        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        // end parsing code
        
        
        ////////////////////////////////////////////////////////
        //                                                    //
        // TODO: use cmdTokens, count, parallel, and timeout  //
        // to implement the rest of closh                     //
        //                                                    //
        // /////////////////////////////////////////////////////
        
        if (parallel == TRUE) { // Run the program parallelly
          int counter = 0;
          clock_t myTimer = clock(); // timeout setup

          while(counter < count){ //Repeat until the amount of counts
            pid_t pid = fork(); // create a child process with fork function
            if(pid == 0){ //if fork succeeds
              execvp(cmdTokens[0],cmdTokens); // execute
            } else if(pid < 0){ //if it fails, prints out "it failed"
              printf("Fork Fail\n");
            } else { //otherwise repeat until it's finished by incrementing counter
              counter++;
              // printf("%d\n", counter); //see how many times the process executes
              if (myTimer < timeout){ //if Timer is less than given timeout terminate the process
              kill(pid,SIGKILL); //kill the process 
              // NEED TO IMPLEMENT LOOPS TO DETERMINE TO SET UP AS WHEN TIMER IS LESS THAN TIMEOUT IT EXECUTES UNTIL THAT
              //BUT KINDA STUCK... AND NOT SURE WHERE TO GO.. :( 
            }
            }
          }
          

        } else { // RUN IT SEQUENTIALLY
        int counter = 0;
        clock_t myTimer = clock();
          while(counter < count){
            pid_t pid = fork(); 
            if(pid == 0){
              execvp(cmdTokens[0],cmdTokens);
            } else if(pid < 0){
              printf("Fork Fail\n");
            } else {
              waitpid(pid, NULL, 0); // Sequentially requires to wait for other process to finish
              counter++;
              if (myTimer < timeout){
              kill(pid,SIGKILL);
            }
            }
          }
        }

    // just executes the given command once - REPLACE THIS CODE WITH YOUR OWN
    exit(1);      
    }
}

