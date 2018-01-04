I have implemented a simple command line intepreter for my university class. This intepreter supports both interactive and 
batch mode. You can enter many commands in one line separated by either a ; or &&. To exit the intepreter use the quit 
command. If you happen to place the quit command between two other commands the intepreter will exit after the execution of 
all the commands which are on the same line as the quit command. To compile enter: `make myshell` via the command line 
inteface. For interactive mode run the executable file like this: `./myshell` and for batch mode like this: 
`./myshell batch_file_name`. 

Feel free to play around and send me any comments or code improvements you want!
