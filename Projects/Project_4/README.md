Create a C program which periodically writes to a `users.log` file, the users and processes currently in the system.
The program should create a child process responsible to write the data to the file. The parent process should use `alarm`,
every T seconds in order for the writing to take place. The child process receives as input the list of users and processes
and sends them to the parent process via `pipe` in order to write them to the file. 
