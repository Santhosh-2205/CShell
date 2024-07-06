# Description
```
Specification 1 : Display Requirement
  prompt.h
  prompt.c

Specification 3 : warp
  warp.c
  warp.h

Specification 4 : peek
  peek.c
  peek.h
  list.c
  list.h

Specification 5 : pastevents commands
  pastevents.c
  pastevents.h

Specification 7 : proclore
  proclore.c
  proclore.h

Specification 8 : seek
  seek.c
  seek.h  
```
To run clone/download zip.
Go to the terminal and run "make" followed by "./a.out"

# Algorithm
Spaces are added before and after special symbols like ;,&,<,>,| and are tokenized by white spaces(and tabs). There by resulting arguments(args)  
These args are now provided to __execute_cmd__ function, which splits args upon ; and & and pass each one to __pipe_seperation__ function  
__pipe_seperation__ dissects based on pipe and pass each one to __redirection_handler__ making sure piping rules are obeyed.  
__redirection_handler__ checks for redirection and redirectes accordingly and passes final command to foreground/background function to execute. They execute the command


# Assumptions

1)If a command contains pastevents in it that complete comand is not stored in it.
 
For ex: 1) pastevents purge; ls

2)If no previous warps are performed warp - gives No such directory

3)There can be maximum 50 background process running simultaneusly.

4)Exit is not stored in pastevensts

5)In seek for dir strict matches are considered, while for files strict matches excluding extensions in considered

6)For neonate assuming time to be integer

7)Not considering quotes for system commands





