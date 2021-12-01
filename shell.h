#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

char** parse_args(char *line);
void useCd(char *line);
int command(char ** args);
void run (char * test);
int main(int argc, char* argv[]);
