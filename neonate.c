#include"headers.h"

void get_latest_pid() 
{
    int fd=open("/proc/loadavg",O_RDONLY);
    if(fd<0)
    {
        perror("Unable to open file");
        return;
    }
    char buffer[1000]={'\0'};
    read(fd,buffer,1000);
 
    char* token = strtok(buffer," ");
    for(int i = 0 ; i < 4 ; i++)
    {
        token = strtok(NULL," ");
    }
    printf("%s\n",token);
 
    close(fd);
}

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
*/
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
*/
void neonate(int num) {
    char *inp = malloc(sizeof(char) * 100);
    char c;

    int stop = 0;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1) {
        die("fcntl");
    }
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
        die("fcntl");
    }
    //double waiting_time=0.0;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();

        get_latest_pid();
        sleep(num); 

        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            }
            else 
            {
                if(c == 'x')
                {
                    stop = 1;
                    break;
                }
            } 
        }
        if(stop == 1)
        {
             if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1)
                die("fcntl");
            disableRawMode();
            return;
        }       
      
        disableRawMode();
    }
    return ;
}

void invoke_neonate(char** args)
{
    if(!args[1] || strcmp(args[1],"-n") != 0)
    {
        neonate_usage();
        return;
    }
    if(!args[2])
    {
        neonate_usage();
        printf("No argument for time\n");
    }
    char *endptr;
    int time = strtol(args[2],&endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        neonate_usage();
        printf("Time must be an integer\n");
        return;
    }
    neonate(time);
}

void neonate_usage()
{
    printf("%s-------------NEONATE USAGE ERROR -------------%s\n",RED,RESET);
    printf("neonate -n <time>\n");
    printf("%s-------------NEONATE USAGE ERROR -------------%s\n",RED,RESET);
}
