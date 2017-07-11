# Shell

MyShell

Submitted by John Spinelli, jspinell@bu.edu

Group Members:
Mike Haley, mhaley10@bu.edu
Clayton Ezzell, cezzel@bu.edu

myshell.c
- Worked on by Mike Haley. Additional debugging by John Spinelli. Limitations to the implementation include:
-cannot run redirection commands with pipes
e.g. cannot do cmd > output file | cmd2
-cmd1 | cmd2 | cmd3 runs correctly but terminates process, need to run ./a.out to receive next prompt
-No control-d exit. 

matmult_p.c
- Worked on by Clayton Ezzell, Additional debugging by John Spinelli. No known limitations to our knowledge of the implementation. 

multiply.c
- Worked on by Clayton Ezzell. No known limitations to our knowledge of the implementation.

matformatter.c
- Worked on by John Spinelli. No known limitations to our knowledge of the implementation. 

makefile
- Worked on by John Spinelli. No known limitations to our knowledge of the implementation. 

Features

myShell.c
-supports every type of command specified in “A Basic Shelk”
e.g. cmd
cmd; cmd
cmd > output file
cmd1 | cmd 2 | cmd 3 
etc

-signal handling for SIGINT and zombie children
-can take input from terminal or input from file, filename must be specified at top of myshell.c in open call



The remaining files operate as assignment suggests. 


