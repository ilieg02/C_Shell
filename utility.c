/*
    Name: Ilie Gabuja
    Student Number: 27231
    I, Ilie Gabuja, agree and accept DCU Academic Integrity Policy and Plagiarism Policy.
*/

// this file utility.c has been made for the purpose of not storing everything in the main function in the simpleshell.c file therefore i find that in this
// manner the code is more secure and not as pronable to buffer overflow and other error that may arise

#include "simpleshell.h"          // functions rules

extern char **environ;             // Environment variables 

// Global variables used for background and command string (local to this file)
static char full_command[MAX_BUFFER];   // store the original command line (for background messages)
static bool is_background = false;      // flag for background execution

// directory changer (unchanged)
void CDchanger(int argc, char *args[]) {
    char cwd[1024];                                   // Buffer for current directory

    if (argc > 1) {                                   // Directory argument given
        if (chdir(args[1]) == 0) {                    // Change the directory succesfully
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                setenv("PWD", cwd, 1);                 // Updating the PWD variable
        } else {
            perror("cd");
        }
    } else {                                           // if no argumment is given just show the current directory
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
    }
}

// easiest function just clear the screen
void CLRscreen() {
    system("clear");               // studying the system library is powerfull!
}

// list the directory , pretty straight forward as it uses ls -al for it!
void DIRshow(int argc, char *args[]) {
    if (argc > 1) {                                         // If argument is given then execute the directory with dir
        char cmd[256];
        sprintf(cmd, "ls -al %s", args[1]);                 // execute ls -al on the given arg
        system(cmd);
    } else {
        system("ls -al");                   // else just display the current directory's contents
    }
}

// environment showing --->> this is from the processes and building a shell in c slides!
void ENVIRONshow() {
    for (int i = 0; environ[i] != NULL; i++)
        printf("%s\n", environ[i]);         //print each environment variable after looping through each 
}

// echo printer, or display the args
void ECHOshow(int argc, char *args[]) {
    for (int i = 1; i < argc; i++)                             // print each argument followed by a space
        printf("%s ", args[i]);
    printf("\n");
}

// display the help manual for the information about the shell commands and etc
void HELPshow() {
    char *shell = getenv("shell"); // path of the executable shell
    char path[1024]; // copy the path so we can change it
    strcpy(path, shell);

    char *last = strrchr(path, '/'); // path containing the binary
    if (last) *last = '\0';

    strcat(path, "/../manual/readme.txt"); // add the path to the manual

    char cmd[1280]; // build the command and run it
    sprintf(cmd, "more < %s", path);
    system(cmd);
}

// pause the shell and continue to work after the Enter key is pressed!
void PAUSEshow() {
    printf("I dare you to press the Enter to continue!!!"); // Prints the prompt message to the screen
    fflush(stdout); // Forces the message to appear immediately (without waiting for a newline)

    FILE *tty = fopen("/dev/tty", "r"); // Tries to open the terminal device directly // /dev/tty always refers to the current terminal
    if (tty) {  // Checks whether opening the terminal succeeded
        while (fgetc(tty) != '\n');     //  reads characters one by one from the terminal until it reads a newline
        fclose(tty); // then close the file
    } else {
        // Fallback (interactive mode only)
        while (getchar() != '\n');
    }
}

// function for quitting the shell
void QUITshow() {
    printf("Bye, See ya next Time!\n");  // BYE!!
    exit(EXIT_SUCCESS);                  // Success Exiting
}


// function to check whether a command is internal
int is_the_command_internal(char *cmd) { // takes a command built-in arg
    if (!cmd) return 0;
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "clr") == 0 || // then check wether the command is valid and return true 
            strcmp(cmd, "dir") == 0 || strcmp(cmd, "environ") == 0 ||
            strcmp(cmd, "echo") == 0 || strcmp(cmd, "help") == 0 ||
            strcmp(cmd, "pause") == 0 || strcmp(cmd, "quit") == 0);
}

// function for executing another external command
void execute_external_command(int argc, char *argv[]) {
    full_command[0] = '\0'; // copy the full command line
    for (int i = 0; i < argc; i++) {
        strcat(full_command, argv[i]);  // concatenate the argvs
        if (i < argc - 1) {
            strcat(full_command, " ");
        }
    }

    // the below is a list of the arguments which will be modified later
    char *local_args[MAX_ARGS]; //buffer
    int local_argc = argc; // local count
    for (int i = 0; i < argc; i++) {
        local_args[i] = argv[i];
    }
    local_args[local_argc] = NULL;

    // background character checker for the character '&'
    is_background = false;
    if (local_argc > 0 && strcmp(local_args[local_argc - 1], "&") == 0) {
        is_background = true;
        local_args[local_argc - 1] = NULL;
        local_argc--; // decrement
    }

    // redirection symbols checker for '>', '>>' and '<'
    int in = -1, out = -1, append = 0;
    for (int i = 0; i < local_argc; i++) {
        if (strcmp(local_args[i], "<") == 0) { // checks for <
            in = i;
        }
        else if (strcmp(local_args[i], ">") == 0) { // checks for >
            out = i;
            append = 0; 
        }
        else if (strcmp(local_args[i], ">>") == 0) { // checks for >>
            out = i;
            append = 1;
        }
    }

    // Fork the process
    pid_t pid = fork(); // fork function uses, clone() system call from the notes
    if (pid == -1) { // checks wether the pid is non valid
        perror("forking error"); // triggersd an error if non valid
        return;
    }

    if (pid == 0) {  // Child process
        setenv("PARENT", getenv("shell"), 1); // requirement acomplished as per project statements

        // input redirection setter
        if (in != -1 && in + 1 < local_argc) {
            if (freopen(local_args[in + 1], "r", stdin) == NULL) {
                perror("Failed input redirection");
                exit(EXIT_FAILURE); // exit system call
            }
            //remover of '<' from the arg list
            local_args[in] = NULL;
            local_args[in + 1] = NULL;
        }

        // output redirection setter
        if (out != -1 && out + 1 < local_argc) {
            const char *mode = append ? "a" : "w";
            if (freopen(local_args[out + 1], mode, stdout) == NULL) {
                perror("Failed output redirection");
                exit(EXIT_FAILURE);
            }
            // remover of '>'/'>>' from the arg list
            local_args[out] = NULL;
            local_args[out + 1] = NULL;
        }

        // clean arg list initializer
        char *clean_args[MAX_ARGS]; // max args
        int clean_argc = 0;
        for (int i = 0; i < local_argc; i++) {
            if (local_args[i] != NULL) // checks wether its null
                clean_args[clean_argc++] = local_args[i]; // counter
        }
        clean_args[clean_argc] = NULL; // nuller

        if (clean_argc == 0) {
            exit(EXIT_SUCCESS); // exit sys call
        }

        // now finally for executing the internal or external command
        if (is_the_command_internal(clean_args[0])) {
            if (strcmp(clean_args[0], "cd") == 0) {
                CDchanger(clean_argc, clean_args);
            }
            else if (strcmp(clean_args[0], "clr") == 0) {
                CLRscreen();
            }
            else if (strcmp(clean_args[0], "dir") == 0) {
                DIRshow(clean_argc, clean_args);
            }
            else if (strcmp(clean_args[0], "environ") == 0) {
                ENVIRONshow();
            }
            else if (strcmp(clean_args[0], "echo") == 0) {
                ECHOshow(clean_argc, clean_args);
            }
            else if (strcmp(clean_args[0], "help") == 0) {
                HELPshow();
            }
            else if (strcmp(clean_args[0], "pause") == 0) {
                PAUSEshow();
            }
            else if (strcmp(clean_args[0], "quit") == 0) {
                QUITshow();
            }
            exit(EXIT_SUCCESS);
        } else {
            // the bellow executes any external command
            execvp(clean_args[0], clean_args); // execvp as from the lab
            perror("Non valid external command");
            exit(EXIT_FAILURE); // another syscall
        }
    } else {  // Parent process
        if (!is_background) { // checks if its not a background process
            int status;
            waitpid(pid, &status, 0); // wait for the thing now
        } else {
            printf("The command: [%s] is now running in the background with the PID [%d]!\n", full_command, pid);
        }
    }
}