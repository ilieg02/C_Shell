#------- Decription -------#

SimpleShell - Stage 1 Help Manual
SimpleShell is a command line interpreter written in the C programming language. 
It supports built-in commands, I/O redirection and background execution.

#------- Building the shell -------#

To build the shell:
    make
To run:
    ./bin/simpleshell

#------- INTERNAL COMMANDS -------#

cd [directory] --> Change directory
clr --> Clear screen
dir [directory] --> List directory files, it is the same as ls -al from the standard Linux Shell
environ --> Show all environment variables
echo [text] --> Prints text to screen
help --> Show the help information from the manual
pause --> Pause shell until 'Enter' key is pressed
quit --> Exit the shell and ends all shell processes

#------- USAGE OF THE ABOVE COMMANDS -------#

---> To change directory: just [cd] or [cd] [path to the directory] for any other directory
---> To print text to screen: [echo] [text]
---> To list files: [dir] or [dir] [path to the directory]
---> To clear screen: [clr]
---> To view environment: [environ]
---> to pause the shell: [pause]
---> To exit: [quit]

#------- Batch Mode USAGE-------#

Run the shell executable with a file argument;
where the file is a basic batchfile which contains a set of command lines for the shell to process.
----> ./simpleshell batchfile


#------- I/O Redirection-------#

Supports input (<) and output (>, >>) redirection

USAGE:
    echo hello > helloworld.text    # adds hello to helloworld.txt
    echo world >> helloworld.txt    # appends world to helloworld.txt
    dir > info.txt                  # internal command 'dir' runs the command and outputs the result to the info.txt file

#------- Background execution -------#

Append the character (&) at the end of a line to run a program in the background.

USAGE:
    sleep 5 &                       # sleep runs in the background and the shell still works

#------- External Commands -------#

For any external commands that are not part of the built-ins commands, the shell forks a child
process and uses execvp() to run it.

#------- Error Handling -------#

For each error that may arise from the system calls(fopen, chdir, fork, execvp, etc.)
The shell checks the return values of them and prints out the appropiate error messages using perror().

#------- ACKNOWLEDGEMENT TO DCU'S ACADEMIC INTEGRITY & PLAGIARISM POLICY -------#

Name: Ilie Gabuja
Student Number: 27231
I, Ilie Gabuja, agree and accept DCU Academic Integrity Policy and Plagiarism Policy.



