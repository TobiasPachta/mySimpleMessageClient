#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "simple_message_client_commandline_handling.h"
#include "logger.h"

//typedef void (* smc_usagefunc_t) (FILE *, const char *, int);
void usagefunc(FILE *stream, const char *cmnd, int exitcode);

int main(const int argc,const char *argv[]) {

    const char *serverIP;
    const char *port;
    const char *user;
    const char *message;
    const char *image;
    int verbose;

    //TODO: read args
    // Fragen wie das mit smc_usagefunc_t funktioniert, weil wenn man ein typ defenieren muss muss der compiler C11 + sein wir sind gerade auf C99
    smc_parsecommandline(argc,argv,usagefunc,&serverIP,&port,&user,&message,&image,&verbose);

    //Check if serverIp is ipv4 or ipv6
    struct addrinfo hint, *res= NULL;

    memset(&hint, '\0', sizeof(hint));

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;

    int success = getaddrinfo(serverIP, NULL, &hint, &res);
    if(success < 0)
        logMessage("Something went wrong at getaddrinfo");

    //create sockaddr
    struct sockaddr *socketAddress;
    socklen_t addressLength = sizeof(struct sockaddr) - 1;

    memset(&socketAddress, 0 ,sizeof(struct sockaddr));

    //create tcp socket
    int tcpSocketFD = 0;

    if(res->ai_family == AF_INET) {
        tcpSocketFD = socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
        socketAddress->sa_family = AF_INET;
    }
    else if(res->ai_family == AF_INET6) {
        socketAddress->sa_family = AF_INET6;
        tcpSocketFD = socket(AF_INET6, SOCK_STREAM, SOCK_NONBLOCK);
    }

    if(tcpSocketFD < 0)
        logMessage("Something went wrong while creating the socket.");

    //connect to Server
    strncpy(socketAddress->sa_data, serverIP, addressLength);

    success = connect(tcpSocketFD, (struct sockaddr *) &socketAddress, addressLength);

    if(success < 0)
        logMessage("Something went wrong while connecting to socket");

    //WENN auch lesen dann muss man tcpSocketfd dublicaten und dann ein eigenen filepointerread erstellen
    FILE *fpw = fdopen(tcpSocketFD, "w");
    //write on Socket
    if(fprintf(fpw, "%s", message))
        logMessage("Something went wrong with writing on socket");
    fflush(fpw);

    shutdown(tcpSocketFD, SHUT_WR);

    //close socket
    fclose(fpw);
    close(tcpSocketFD);

    return 0;
}

void usagefunc(FILE *stream, const char *cmnd, int exitcode){
    fprintf(stream, "%s", cmnd);
    exit(exitcode);
}