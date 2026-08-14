/*
    Name: Ilie Gabuja
    Student Number: 27231
    I, Ilie Gabuja, agree and accept DCU Academic Integrity Policy and Plagiarism Policy.
*/

#include "simpleshell.h"          // header file with functions rules or prototypes yk

#define SEPARATORS " \t\n"        // // token separators reference from the book slides

int main(int argc, char *argv[]) {
    //  change the path of this executable to the shells environment variable

    char path[512];                               // buffer for the path
    if (realpath(argv[0], path) != NULL) {        // Get path of the program
        setenv("shell", path, 1);                  // Set finally
    }

    char *username = getenv("USER");                     // find and get the username from the environment variables
    if (!username) {
        username = "user";
    }


    // message for the hackers
    printf("  ____  _                 _     ____  _          _ _ \n");
    printf(" / ___|(_)_ __ ___  _ __ | |_  / ___|| |__   ___| | |\n");
    printf(" \\___ \\| | '_ ` _ \\| '_ \\| __| \\___ \\| '_ \\ / _ \\ | |\n");
    printf("  ___) | | | | | | | |_) | |_   ___) | | | |  __/ | |\n");
    printf(" |____/|_|_| |_| |_| .__/ \\__| |____/|_| |_|\\___|_|_|\n");
    printf("                   |_|                                \n");

    // iNPUT SOURCE 
    FILE *input = stdin;
    if (argc == 2) {                                      // if args are given
        input = fopen(argv[1], "r");                       // open file for reading 
        if (!input) {                                      // If opening failed then make an error and exit
            perror("ERROR");
            return 1;
        }
    }
    //----------------------//
    char buf[MAX_BUFFER];                                 // line buffer
    char *args[MAX_ARGS];                                 // pointers to arg strings
    char **arg;                                            // Working pointer thru args
    int arg_count;                                         // arg nr

    while (!feof(input)) {                                 // loop until the end of file
        if (input == stdin) {                              // If from stdin
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)          // current directory
                printf("%s:%s> ", username, cwd);           // Print username with its directory
            else
                printf("%s:shell> ", username);
            fflush(stdout);                                 // immeditaly print it out
        }

        // read from input
        if (!fgets(buf, sizeof(buf), input)) {
            break; // if read fails just break the loop or exit
        }
        // from the example of the tokenizer
        arg = args;
        *arg++ = strtok(buf, SEPARATORS);                   // tokenise input
        while ((*arg++ = strtok(NULL, SEPARATORS)));
        arg_count = arg - args - 1;                         // last entry will be NULL 

        // now go and find the command with the function!
        if (args[0] != NULL) {                              // if at least one token is present

            // checker for the redirection symbols
            int has_redir = 0;
            for (int i = 0; i < arg_count; i++) {
                if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
                    has_redir = 1;
                    break;
                }
            }
            if (has_redir) {
                execute_external_command(arg_count, args); // if true then execute it
                continue;
            }

            // now simply compare the token (arg) with the function and then if succeds then run the executable method
            if (strcmp(args[0], "cd") == 0)
                CDchanger(arg_count, args);                  // directory changer
            else if (strcmp(args[0], "clr") == 0)
                CLRscreen();                                  // Clear screen
            else if (strcmp(args[0], "dir") == 0)
                DIRshow(arg_count, args);                     // directory lister
            else if (strcmp(args[0], "environ") == 0)
                ENVIRONshow();                                 // Show environment variables
            else if (strcmp(args[0], "echo") == 0)
                ECHOshow(arg_count, args);                     // Echo text
            else if (strcmp(args[0], "help") == 0)
                HELPshow();                                     // help manual
            else if (strcmp(args[0], "pause") == 0)
                PAUSEshow();                                    // Pause until Enter key is pressed
            else if (strcmp(args[0], "quit") == 0) {
                QUITshow();                                     // quit shell
                if (input != stdin) fclose(input);              // Close batch file if open
                return 0;                                        // Exit successfully
            }
            else
                // handle external commands, redirection, and background
                execute_external_command(arg_count, args);
        }
    }

    // clean for the batch mode
    if (input != stdin) {
        fclose(input);
    }
    return 0;
}