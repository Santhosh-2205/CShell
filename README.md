# Custom Linux Shell

## Overview
This project is an implementation of a custom Linux shell, developed as part of the Operating Systems course. The shell, named C Shell, mimics the functionality of a typical UNIX shell, providing a user interface for command execution and process management

## Features

- **Core Shell Commands**: Implements essential shell commands such as `cd`, `ls`, `find`, and `history`. Supports `;` or `&` separated list of commands.
- **Background Processes**: Supports the execution of commands in the background, allowing for multitasking.
- **I/O Redirection**: Facilitates input and output redirection for commands using `>` and `<`.
- **Piping**: Allows for piping between commands using the `|` operator, enabling the output of one command to serve as the input for another.
- **Process Management**: Supports process management command `kill` to send signals to processes.

## Getting Started

### Prerequisites

To build and run this project, you will need:
- GCC (GNU Compiler Collection) installed on your system.
- A UNIX-like operating system (Linux, macOS).

### Installation

1. Clone the repository:
   ```
   git clone https://github.com/Santhosh-2205/CShell
   cd CShell
   ```

2. Compiling and running:
   ```
   make
   ./a.out
   ``` 
 - You have now entered the custom shell, which will be displayed as `<Username@SystemName:~>`. Here, Username and SystemName will correspond to your actual system username and hostname.  
 - The directory where the shell is spawned is the home directory, represented as `~`. While directories within the home directory(This shell's home) are shown relative to `~`, you can navigate to directories beyond the home directory as well for which real path(Your system's) are shown.

## Usage

### 1. Warp (cd)
The `warp` command changes the directory that the shell is currently in and prints the full path of the working directory after changing. The directory path or name can be provided as an argument to this command.
- **Basic Usage**:
  ```bash
  warp /path/to/directory
- Supports `.`,  `..`, `~`, and `-` flags

### 2.Peek (ls)
The `peek` command lists all files and directories in the specified directories in lexicographic order. By default, peek does not show hidden files.
- **Basic Usage**:
    ```bash
    peek /path/to/directory
- Supports `-l` and `-a` flags

### 3.Pastevents (history)
The pastevents command stores and outputs the atmost 15 most recent command statements given as input to the shell, with persistence over different shell runs.
- **Basic Usage**:
    ```bash
    pastevents
    Displays the most recent commands(at max 15)
- **Purge**:
    Clears all the pastevents currently stored.
    ```bash
    pastevents purge

- **Execute**:
    Executes the command at the specified position in pastevents.
    ```bash
    pastevents execute <index>

### 4.Background Process
Any command invoked with & is treated as a background command.
- **Basic Usage**:
    ```bash
    some_command &

### 4.Proclore
The proclore command is used to obtain information regarding a process. If no argument is provided, it prints the information of the shell.
- **Basic Usage**:
    ```bash
    proclore <pid>
    
### 5.Seek (find)
The seek command looks for a file or directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths of all matching files or directories, with files in green and directories in blue.
- **Basic Usage**:
    ```bash
   seek <name> [path]
- Flags: `-d` (directories only), `-f` (files only), `-e` (print or change to single match).

### 6.Ping (kill)
The ping command sends a signal corresponding to signal_number (modulo 32) to processes identified by their PID.
- **Basic Usage**:
    ```bash
    ping <pid> <signal_number>

### 7.iMan
Fetches the man page for the given command from http://man.he.net/ by performing DNS resolution, opening a TCP socket to the IP address, sending a GET request, reading the website's body.
- **Basic Usage**:
    ```bash
    iMan <command_name>

### 8.System Commands
Can execute the other system commands present in Bash.

### 9.Additional commands
1. ```bash
   fg <pid>
Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal

2. ```bash
   bg <pid>
Changes the state of a stopped background process to running (in the background).

3. ```bash
   neonate -n [time_arg]
Prints the pid of the most recently created process on the system every [time_arg] seconds until the key ‘x’ is pressed.

### 10.Keyboard Controls

1. **Ctrl - C**
Interrupts any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

2. **Ctrl - D**
Logs out of your shell (after killing all processes) while having no effect on the actual terminal.

3. **Ctrl - Z**
Pushes the running foreground process (if any) to the background and changes its state from "Running" to "Stopped". It has no effect on the shell if no foreground process is running.








