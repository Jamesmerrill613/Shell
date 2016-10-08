# Shell
Operating Systems USU Shell Assignments

Assignment 1
Introduction
A shell is a user program or environment provides for user interaction. The shell is a command language interpreter that accepts commands from standard input (the keyboard) and executes them. A shell may simply accept user commands and pass them off to the operating system to execute them, it may additionally provide mechanisms, such as redirecting the output of one program to the input of another.

Assignment
Write a C++ program that will act as a command line interpreter for Linux. Use the “[cmd]:” string for your prompt. For example, when a user types a line such as…

word1 word2 word3

...the shell interprets 'word1' as the name of an executable program and 'word2' and 'word3' as parameters to be passed to that program in argv. Your shell should accept any number of command line parameters.

The shell should use the directory system in the order specified by the PATH environment variable to locate the executable file (i.e. You don't have to do anything, don't send me an email asking how to do this, it automatically happens). If the file is found, the shell should pass all parameters in argv and be executed. When the executable file has completed or terminated, the shell program should again resume with the next prompt. Your shell program must wait to display the next command prompt until the previous command has finished executing.

The shell should track the amount of time spent executing child processes. Whenever the command 'ptime' is entered into your shell, the number of seconds, accurate to microseconds, spent executing (waiting for) child processes must be displayed.

The shell should keep a history of the previous commands entered, with no maximum hard-coded history length. If the user enters the command 'history', the shell should provide a listing of the complete command shell history. If the user enters the command '^ 10', for example, the 10th command in the history buffer should be executed (a 1 based index).

The command 'exit' is used to terminate your shell program, when entered, your shell program should end.

The following is an example of what the command shell will look like and how programs will be executed.

    [cmd]: ls
          shell
          shell.cpp
    [cmd]: ptime
          Time spent executing child processes: 0 seconds 24 milliseconds and 380 microseconds
    [cmd]:
Notes
You must use fork, execvp, wait and std::chrono to complete this assignment.
Reference for std::chrono : http://en.cppreference.com/w/cpp/chrono (Links to an external site.)
Your shell must accept any number of parameters.
Your shell must not crash when the user enters bad commands or bad command arguments.
File Upload
