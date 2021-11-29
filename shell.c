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
			break;
		}
		
		else if (0) {
		int f = fork();
		int w, status;
		if(f) {
		 w = wait();
		}
	}
	}
	return 0;
}	


