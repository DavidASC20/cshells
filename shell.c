#include "shell.h"
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

void useCd(char *line){
	char* templine = line;
	char dir[100];
	printf("Prev directory: %s\n", getcwd(dir, 100));
	if(chdir(templine) != 0){
		printf("Error %s", strerror(errno));
	}else{
		chdir(templine);
		printf("Current directory:%s\n", getcwd(dir, 100));
	}
}	

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


int main(int argc, char* argv[]){
	while(1){
		char currdir[100];
		printf("%s:) ", getcwd(currdir, 100));
		char test[100];
		fgets(test, 100, stdin);
		char* newline = strchr(test, '\n');
		*newline = 0;
		char** arguments;
		arguments = parse_args(test);
		
		if(strcmp(arguments[0],"cd") == 0){
			useCd(arguments[1]);
		}
		
		else if(strcmp(arguments[0],"exit") == 0){
			printf("Thanks for comin!\n");
			return 0;
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
	return 0;
}	


