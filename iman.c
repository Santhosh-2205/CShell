#include "headers.h"

struct sockaddr_in* resolveHostname(const char *hostname) {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return NULL;
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memcpy(ipv4, res->ai_addr, sizeof(struct sockaddr_in)); 

    freeaddrinfo(res); 

    return ipv4;
}

int openTCPSocket(struct sockaddr_in *ipv4) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    ipv4->sin_port = htons(80);

    if (connect(sockfd, (struct sockaddr *)ipv4, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void sendHttpGetRequest(int sockfd, const char *request) {
    send(sockfd, request, strlen(request), 0);
}

void readWebsiteBody(int sockfd) {
    char buffer[1024];
    int bytesRead;

    int start_printing = 0;
    int is_printing = 0;

    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        for(int i = 0 ; i < bytesRead && !start_printing ; i++)
        {
            if(buffer[i] == 'N' && buffer[i+4] == '\n')   
                start_printing = i;
        }
        for(int i = start_printing ; i < bytesRead; i++)
        {
            if( buffer[i] == '<')
            is_printing = 0;
            else if(buffer[i] == '>')
            is_printing = 1;
            else if(is_printing)
            printf("%c",buffer[i]);
        }
    }
}

int invoke_iman(char** args) {

    if(!args[1])
    {
        iman_usage();
        printf("Please provide command name\n");
        return 0;
    }
    char user_input[256];
    strcpy(user_input,args[1]);

    char request[512];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", user_input);

    const char *hostname = "man.he.net";
    struct sockaddr_in *addr = resolveHostname(hostname);
    if (addr == NULL) {
        return 1;
    }

    int sockfd = openTCPSocket(addr);
    if (sockfd == -1) {
        return 1;
    }

    sendHttpGetRequest(sockfd, request);
    readWebsiteBody(sockfd);

    close(sockfd);

    free(addr);

    return 0;
}

void iman_usage()
{
    printf("%s-------------IMAN USAGE ERROR -------------%s\n",RED,RESET);
    printf("iMan <command_name>\n");
    printf("%s-------------IMAN USAGE ERROR -------------%s\n",RED,RESET);
}
