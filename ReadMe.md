## SimpleShell

SimpleShell is a command line interpreter written in the C programming language. It supports built-in commands, I/O redirection, and background execution.

# 🛠️ Building the Shell

To build:
```Bash

make
```

To run:

```Bash

./bin/simpleshell
```

# 📖 Built-In Commands

    cd [directory] — Change the current working directory.

    Clr — Clear the terminal screen.

    dir [directory] — List directory files (equivalent to ls -al in standard Linux).

    environ — Show all environment variables.

    echo [text] — Print text to the screen.

    help — Show the help information from the manual.

    pause — Pause the shell until the 'Enter' key is pressed.

    quit — Exit the shell and end all shell processes.

# ⚙️ Advanced Features
# External Commands

For any external commands that are not built-in, the shell forks a child process and uses execvp() to run them.
Batch Mode

Run the shell executable with a file argument to process a pre-written set of commands automatically.
``` Bash

./simpleshell batchfile.txt

```
# I/O Redirection

Supports input (<) and output (>, >>) redirection for commands.
``` Bash

echo hello > helloworld.txt   # creates/overwrites file with 'hello'
echo world >> helloworld.txt  # appends 'world' to the file
dir > info.txt                # outputs the internal 'dir' command result to a file
``` 

# Background Execution

Append the & character at the end of a command to run a program in the background.

```Bash

sleep 5 &                     # sleep runs in the background; the shell stays active

```
⚠️ Error Handling

For each error that may arise from system calls (such as fopen, chdir, fork, execvp, etc.), the shell checks their return values and prints the appropriate error messages using perror().
