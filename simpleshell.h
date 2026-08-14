/*
    Name: Ilie Gabuja
    Student Number: 27231
    I, Ilie Gabuja, agree and accept DCU Academic Integrity Policy and Plagiarism Policy.
*/

/////// Stdin & Output -->
#include <stdio.h> 
/////// builtin libraries -->
#include <stdlib.h> 
/////// String editing -->
#include <string.h>
/////// symbolic constants and types, for fork and exec! -->
#include <unistd.h>
/////// for errors -->
#include <errno.h>
/////// Data types -->
#include <sys/types.h>
/////// Process management -->
#include <sys/wait.h>
/////// File control -->
#include <fcntl.h>
/////// Logic -->
#include <stdbool.h>

/////// maximum buffer size for input
#define MAX_BUFFER 1024

/////// max nr of command line arguments 
#define MAX_ARGS 64

extern char **environ; // environment vars


void CDchanger(int argc, char *args[]); // <-- change the directory
void CLRscreen(); // <-- clear the screen
void DIRshow(int argc, char *args[]); // <-- show the directory contents
void ENVIRONshow(); // <-- show the environment variables
void ECHOshow(int argc, char *args[]); // <-- display a text or args
void HELPshow(); // <-- display the help manual
void PAUSEshow(); // <-- pause the shell
void QUITshow(); // <-- exit the shell and all its processes
void execute_external_command(int argc, char *args[]); // <-- wrapper