//Names(s): Daniel Austin
//Description: a C program that acts like a bash shell and allows a user to enter input that gets interpreted as a shell command.

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>

# define MAX_INPUT 1024

typedef struct{
	char *args[32];
	int inredirect;
        int outredirect;
        char *infile;
        char *outfile;
} ShellCommand;

void prompt(void);
void getInput(char *buffer, int size);
void tokenizer(char *input, ShellCommand *cmd);
void execute(ShellCommand *cmd);

int main(){

	char input[MAX_INPUT];
	ShellCommand cmd;

	while(1){
		prompt();
		getInput(input, MAX_INPUT);
		if(strcmp(input, "exit") == 0){//quit
                        break;
                }
		tokenizer(input, &cmd);
		if(cmd.args[0] != NULL && strcmp(cmd.args[0], "cd") == 0){
			if(cmd.args[1] == NULL){ // go home
				char *home = getenv("HOME");
				if(home == NULL){
					perror("cd");
				}
				else if(chdir(home) != 0){
					perror("cd");
				}
			}
			else{ // go to dir of args[]
				if(chdir(cmd.args[1]) !=0){
					perror("cd");
				}
			}
			continue;
		}

		execute(&cmd);
	}
	return 0;
}
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
/*
    A function that parses through the user input.
*/
void tokenizer(char *input, ShellCommand *cmd){
	char *token;
	cmd->inredirect = 0;
	cmd->outredirect = 0;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	token = strtok(input, " ");//tokenize
        int i = 0;
        while(token != NULL){//output tokens
                cmd->args[i++] = token;
                token = strtok(NULL, " ");
	}
	cmd->args[i] = NULL; 
	int k = 0;	
        for(i = 0; cmd->args[i] != NULL; i++){
                 if(strcmp(cmd->args[i], "<") == 0){
                         cmd->inredirect = 1;
                         cmd->infile = cmd->args[i + 1];
			 i++; //skip file name
			 continue;
                 }
                 if(strcmp(cmd->args[i], ">") == 0){
                         cmd->outredirect = 1;
                         cmd->outfile = cmd->args[i + 1];
			 i++; //skip file name
			 continue;
                }
		cmd->args[k++] = cmd->args[i];
         }
	 cmd->args[k] = NULL;
}
/*
    A function that executes the command.
*/
void execute(ShellCommand *cmd){
	int pid = fork();

	if(pid == 0){ //child runs
		if(cmd->outredirect){
			int fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); // chatgpt helped with this line.
			if(fd < 0){
				perror("open");
				_exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		if(cmd->inredirect){
                        int fd = open(cmd->infile, O_RDONLY);
                        if(fd < 0){
                                perror("open");
                                _exit(1);
                        }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                }

		execvp(cmd->args[0], cmd->args); //run the command
		perror("execvp");
		_exit(1);
	}
	else if(pid > 0){ //parent waits
		waitpid(pid, NULL, 0);
	}
	else{ //error
		perror("Fork function failed");
	}
}
