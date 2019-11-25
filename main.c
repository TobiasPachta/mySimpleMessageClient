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

int main(int argc, char * argv[]) {

    char * server = "";
    char * port = "7490";
    char * user = "ic18b028";
    char * message = "message";
    char * image = "";
    int verbose = 0;


    smc_parsecommandline(argc,argv,smc_usagefunc_t,&server,&port,&user,&message,&image,&verbose);

}