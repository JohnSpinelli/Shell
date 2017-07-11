{\rtf1\ansi\ansicpg1252\cocoartf1344\cocoasubrtf720
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural

\f0\b\fs36 \cf0 CS 410 Assignment 2 \'97 MyShell\
\

\b0\fs24 Submitted by John Spinelli, jspinell@bu.edu\
\
Group Members:\
Mike Haley, mhaley10@bu.edu\
Clayton Ezzell, cezzel@bu.edu\
\

\b myshell.c\
-
\b0  Worked on by Mike Haley. Additional debugging by John Spinelli. Limitations to the implementation include:\
-cannot run redirection commands with pipes\
e.g. cannot do cmd > output file | cmd2\
-cmd1 | cmd2 | cmd3 runs correctly but terminates process, need to run ./a.out to receive next prompt\
-No control-d exit. \
\

\b matmult_p.c
\b0 \
- Worked on by Clayton Ezzell, Additional debugging by John Spinelli. No known limitations to our knowledge of the implementation. \
\

\b multiply.c\

\b0 - Worked on by Clayton Ezzell. No known limitations to our knowledge of the implementation.\
\

\b matformatter.c\

\b0 - Worked on by John Spinelli. No known limitations to our knowledge of the implementation. \
\

\b makefile
\b0 \
- Worked on by John Spinelli. No known limitations to our knowledge of the implementation. \
\

\fs36 \ul Features\
\

\b\fs24 \ulnone myShell.c
\b0 \
-supports every type of command specified in \'93A Basic Shelk\'94\
e.g. cmd\
cmd; cmd\
cmd > output file\
cmd1 | cmd 2 | cmd 3 \
etc\
\
-signal handling for SIGINT and zombie children\
-can take input from terminal or input from file, filename must be specified at top of myshell.c in open call\
\
\
\

\b The remaining files operate as assignment suggests. 
\b0 \
\
\
}