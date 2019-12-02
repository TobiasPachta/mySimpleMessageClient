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

    smc_parsecommandline(argc,argv,usagefunc,&serverIP,&port,&user,&message,&image,&verbose);

    struct addrinfo hint, *res= NULL;

    memset(&hint, '\0', sizeof(hint));

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_socktype = SOCK_STREAM;

    int success = getaddrinfo(serverIP, port, &hint, &res);
    if(success < 0){
	    logError(gai_strerror(success));
        logError("Something went wrong at getaddrinfo");
	    return 1;
	}

    //create tcp socket
    int tcpSocketFD = 0;

    tcpSocketFD = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(tcpSocketFD < 0) {
        logError("Something went wrong while creating the socket");
	    return 1;
	}

    //connect to Server
    success = connect(tcpSocketFD, res->ai_addr, res->ai_addrlen);

    if(success < 0){
        logError("Something went wrong while connecting to socket");
        close(tcpSocketFD);
	    return 1;
	}

    logMessage("Connected to Server");

    //WENN auch lesen dann muss man tcpSocketfd dublicaten und dann ein eigenen filepointerread erstellen
    FILE *fpw = fdopen(tcpSocketFD, "w");

    if(fpw == NULL){
        logError("Something went wrong a opening a writter file descriptor");
        close(tcpSocketFD);
        return 1;
    }

    //write on Socket
    if(fprintf(fpw, "%s \n", message) < 0){
        logError("Something went wrong with writing on socket\n");
        fclose(fpw);
        close(tcpSocketFD);
	    return 1;
	}

    logMessage("Message Sent:");
    logMessage(message);

    fflush(fpw);

    shutdown(tcpSocketFD, SHUT_WR);

    //close socket
    fclose(fpw);
    close(tcpSocketFD);

    return 0;
}

void usagefunc(FILE *stream, const char *cmnd, int exitcode){
    fprintf(stream, "Usage: %s -s <server> -u <user> -m <message> -p <port> -i <imageURL>\n", cmnd);
    exit(exitcode);
}
