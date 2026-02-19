// Name(s): Daniel Austin
// Description: A program that acts like a bash shell. That allows 
// a user to enter input that gets interpreted as a shell command.

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>

# define MAX_INPUT 1024

void prompt(void);
void getInput(char *buffer, int size);
void tokenizer(char *input, char *args[]); //do later
void execute(char *args[]); //do later

int main(){

        char input[MAX_INPUT];
        char *args[32];

        while(1){
                prompt();
                getInput(input, MAX_INPUT);
                if(strcmp(input, "exit") == 0){//quit
                        break;
                }
	}
}
//////// Some function ideas: ////////////
// Note: Some code is reflected in main that represents these functions,
// but it is up to you to determine how you want to organize your code.

/*
    A function that causes the prompt to
    display in the terminal
*/

void prompt(void){ //gets cwd and prints prompt
        char cwd[MAX_INPUT];
        getcwd(cwd, MAX_INPUT);
        printf("%s$ ", cwd);
}
/*
    A function that takes input from the user.
    It may return return the input to the calling statement or
    store it at some memory location using a pointer.
*/

void getInput(char *buffer, int size){ // get user input
        if(fgets(buffer, size, stdin) !=NULL){
                buffer[strcspn(buffer, "\n")] = '\0';//remove newline
        }
        else{
                buffer[0] = '\0';
        }
}


