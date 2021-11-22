#include "shell.h"

int main(){
    char line[100] = "ls -a -l";
    char **args = parse_args(line);
    int err = execvp(args[0], args);
    if(err != 0){
        printf("Error, Message is: %s\n", strerror(errno));
        return err;
    }
    return 0;
}