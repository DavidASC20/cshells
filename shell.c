#include "shell.h"
char** parse_args(char *line){
    char **args = calloc(5, sizeof(char *));
    args[0] = strsep(&line, " ");
    int num;
    for(num = 1; num < 5; num++){
        args[num] = strsep(&line, " ");
    if (args[num] == NULL) {
      break;
    }
  }return args;
}