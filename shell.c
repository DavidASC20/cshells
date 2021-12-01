#include "shell.h"

//parse_args takes in the command line arguments and parses them into an array of usable strings for the rest of the program.
char** parse_args(char *line){
	int count = 1;
	char* lineCopy = line; 
	while(*lineCopy){
		if(*lineCopy == ' ') count++;
		lineCopy++;
	}
    	char **args = calloc(count + 1, sizeof(char *));
    	args[0] = strsep(&line, " ");
    	int num;
    	for(num = 1; num < count+1; num++){
        	args[num] = strsep(&line, " ");
    		if (args[num] == NULL) {
      			break;
    		}
  	}return args;	
}
//useCd takes in a parsed argument, and attempts to change directory.  It begins by printing the current directory, and tries to cd into the argument.  If the argument doesnt exist, then an error is printed, and the user stays on the current directory.  If the argument exists, then the directory is changed to the argument, and the new directory is printed out.    
void useCd(char *line){
	char* templine = line;
	char dir[100];
	printf("Prev directory: %s\n", getcwd(dir, 100));
	if(chdir(templine) != 0){
		printf("Error %s\n", strerror(errno));
	}else{
		chdir(templine);
		printf("Current directory:%s\n", getcwd(dir, 100));
	}
}	
//command takes in the array of parsed arguments, and forks the parent process.  It then executes the first argument with the other arguments as variables.  If execvp fails, it will return an error. 
int command(char ** args) {
	int f = fork();
	int w, status;
	if (f) {
		w = waitpid(f, &status, 0);
		return WEXITSTATUS(status);
	} else {
	int err = execvp(args[0], args);
	if(err != 0) {
		printf("error, Message is: %s\n", strerror(errno));
		return err;        
	}
	exit(0);
	}
    return 0;
}

// run takes in the command argument line and then calls parse_args. It checks if the first argument is cd or exit so that it can either call useCd or exit. It then checks for a pipe. If there is a pipe, it forks the command. In the forked process, it makes the pipe equal to null to split the arguments into ones before the pipe and after. It then reads the output of the first arguments and writes to the input of the second arguments. After that, it checks for redirection. It opens the file and duplicates the file into STDIN or STDOUT.
void run(char* test){
char** arguments;
		char test_copy[1000];
		strcpy(test_copy, test);
		arguments = parse_args(test);
		
		if(strcmp(arguments[0],"cd") == 0){
			useCd(arguments[1]);
		}
		
		else if(strcmp(arguments[0],"exit") == 0){
			printf("Thanks for comin!\n");
			exit(0);
		}
		else if(strchr(test_copy, '|') != 0){
			int f = fork();
			int w, status;
			if (f) {
				w = waitpid(f, &status, 0);
			} else {
				char * pipe = strchr(test_copy, '|');
				*pipe = 0;
				FILE* first = popen(test_copy, "r");
				FILE* second = popen(pipe+1, "w");
				char buffer[1000];
				while(fgets(buffer, 1000, first)){
					fputs(buffer, second);
				}
				exit(0);
			}
				
		}
		else{
			char ** arg_copy = arguments;
			int stdin_dup = dup(STDIN_FILENO);
			int stdout_dup = dup(STDOUT_FILENO);
			
			int stdin_redir = -1;
			int stdout_redir = -1;
			while(*arg_copy){
				if(strcmp(*arg_copy, "<") == 0) {
					stdin_redir = open(arg_copy[1], O_RDONLY, 0777);
					dup2(stdin_redir, STDIN_FILENO);
					*arg_copy = 0;
				}
				else if(strcmp(*arg_copy, ">") == 0) {
					stdout_redir = open(arg_copy[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
					dup2(stdout_redir, STDOUT_FILENO);
					*arg_copy = 0;
				}
				else if(strcmp(*arg_copy, ">>") == 0) {
					stdout_redir = open(arg_copy[1], O_WRONLY | O_CREAT | O_APPEND, 0777);
					dup2(stdout_redir, STDOUT_FILENO);
					*arg_copy = 0;
				}
				arg_copy++;
			}
			command(arguments);
			if(stdin_redir > -1) {
				close(stdin_redir);
			}
			if(stdout_redir > -1) {
				close(stdout_redir);
			}
			dup2(stdin_dup, STDIN_FILENO);
			dup2(stdout_dup, STDOUT_FILENO);
		}
}

// It takes in the command line arguments and removes the newline.  It then checks the arguments for a semicolon, and sets a pointer to the beginning.  While a semicolon exists,  it is removed and set to 0, and the command from before the semicolon is run.  From there, a pointer is declared after the once existing semicolon, thereby continuing the script.  It will continue until no more are found, and then the pointer after the existing semicolon is ran.  
int main(int argc, char* argv[]){
	while(1){
		char currdir[100];
		printf("%s:) ", getcwd(currdir, 100));
		char test[1000];
		fgets(test, 1000, stdin);
		char* newline = strchr(test, '\n');
		*newline = 0;
		char * pointer = test;
		while(strchr(test, ';') != 0) {
			char * semicolon = strchr(test, ';');
			*semicolon = 0;
			run(pointer);
			pointer = semicolon + 1;
		}
		run(pointer);
		}
		return 0;
}	


			

