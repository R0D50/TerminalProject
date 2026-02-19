#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define DEBUG 


void change_dir(const char *path){//Used CGPT to find appropriate args for func
	if (chdir(path) != 0){
		perror("command failed");
	} else {
		printf("Directory changed");
	}
   
}
//Looked up how to format arguments for this function
void parse(char *input, char *args[], char *input_file, char *output_file){
	char *token;
	int i = 0; 

	input_file = NULL;
	output_file = NULL;

	token = strtok(input, " ");
	while (token != NULL && i <MAX_ARGS -1){
		if (strcmp(token, "<") == 0){
			input_file = strtok(NULL, " ");
		} else if (strcmp(token, ">") == 0){
			output_file = strtok(NULL, " ");
		}else{
			args[i++] = token;
		}
		token = strtok(NULL, " ");
		}
	args[i] = (NULL, " ");
	
}



int main() {
	char input[MAX_INPUT];
	char *args[MAX_ARGS];
	char cwd[MAX_INPUT];
	char *input_file = NULL;
    char *output_file = NULL;	
	



    while (1) {
        //Print prompt
	char *cwd = getcwd(NULL, 0);

	
        printf("%s:$ ",cwd);
        fflush(stdout);

        //Read input
        if (fgets(input, MAX_INPUT, stdin) == NULL){
        	break;
	}

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        // Exit command
        if (strcmp(input, "exit") == 0){
        	break;
	}

		// change directory
	if (strncmp(input, "cd", 2) == 0){
		char *path = input +3;
		if(strlen(path) == 0){
			path = getenv("HOME");		
		}
	
		change_dir(path);
		fflush(stdout);
	}

	//parse input
	parse(input, args, input_file, output_file);

        
		

        //Fork process
        pid_t pid = fork();

        if (pid == 0) { // Child process

			// REDIRECTION -->
			if (input_file != NULL){
				int fd = open(input_file, O_RDONLY);
				if (fd < 0){
					perror("failed");
					exit(1);
			}
				dup2(fd, STDIN_FILENO);
				close(fd);
		}	//REDIRECTION <--
			if (output_file != NULL){
				int fd2=open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd2 < 0){
					perror("failed");
					exit(1);
				}
				dup2(fd2, STDOUT_FILENO);
				close(fd2);
			}
			
			
			//execute
			if(execvp(args[0], args) == -1){
				perror("Execution failed");
				exit(1);
			}

					
		
				
			
		} else if (pid > 0) {// Parent process
				
		
			wait(NULL);
			exit(1);
			} 
		else {
			perror("Fork failed");
		}
	

		
	}
    return 0;
  }
