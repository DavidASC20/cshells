# cshells

## FEATURES

This shell is a basic implementation of a regular bash shell.  All commands supported by execvp(), as well as cd and exit can be used.

## COMMANDS

``char** parse_args(char *line);``takes in the command line arguments and parses them into an array of usable strings for the rest of the program.

``void useCd(char *line);`` takes in a parsed argument, and attempts to change directory.  It begins by printing the current directory, and tries to cd into the argument.  If the argument doesnt exist, then an error is printed, and the user stays on the current directory.  If the argument exists, then the directory is changed to the argument, and the new directory is printed out.

``int command(char ** args);`` takes in the array of parsed arguments, and forks the parent process.  It then executes the first argument with the other arguments as variables.  If execvp fails, it will return an error. 

``void run (char * test);`` takes in the command argument line and then calls parse_args. It checks if the first argument is cd or exit so that it can either call useCd or exit. It then checks for a pipe. If there is a pipe, it forks the command. In the forked process, it makes the pipe equal to null to split the arguments into ones before the pipe and after. It then reads the output of the first arguments and writes to the input of the second arguments. After that, it checks for redirection. It opens the file and duplicates the file into STDIN or STDOUT.

``int main(int argc, char* argv[]);`` takes in the command line arguments and removes the newline.  It then checks the arguments for a semicolon, and sets a pointer to the beginning.  While a semicolon exists,  it is removed and set to 0, and the command from before the semicolon is run.  From there, a pointer is declared after the once existing semicolon, thereby continuing the script.  It will continue until no more are found, and then the pointer after the existing semicolon is ran.

## REQUIREMENTS

Each argument must be separated by one space, and there cannot be starting or trailing spaces.  For semicolon usage, DO NOT include spaces between the arguments and semicolon. (IE ls;wc)

## KNOWN BUGS

Sometimes, the program requires two exits in order to exit the program.  It doesnt happen everytime, only once in a while.

