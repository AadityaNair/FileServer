/*
    File Server
    Author: Aaditya M Nair (Prometheus)	
    Created On : Monday 02 March 2015 12:24:28 PM IST

    This is the common head for client and server.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "client.c"
#include "server.c"
#include "logger.h"
#include "data.h"

void fork_server();
void fork_client();

int serverpid, clientpid;

int main()
{
    char input[100];
    while(1)
    {
        printf(">>>");
        scanf("%s", input);

        if(!strcmp(input, "quit"))
        {             // check if user quits
            if(serverpid)
                kill(serverpid, SIGKILL);
            if(clientpid)
                kill(clientpid, SIGKILL);
            exit(0);
        }
        else if(!strcmp(input, "list_self"))
        {    // Get user's own filelist.
            FILE *fd=fopen("filelist", "r");
            printf("%s\n", get_buffer(fd));
        }
        else if(!strcmp(input, "start_server"))
        {// Start Server.
            warning("Trying to start server.");
            fork_server();
            continue;
        }
        else if(!strcmp(input, "start_client"))
        {// Start Client
            if(clientpid)
                kill(clientpid, SIGKILL);
            fork_client();
        }
        else
            warning("Wrong Input.");
        printf("\n");
    }
    return 0;
}


void fork_server(){
    int rt=fork();
    if(rt<0){
        alert("Unable to fork server.Exiting..");
        exit(-1);
    }
    if(rt==0){ // Start the server
        printf("Starting Server.\n");
        server();
    }
    else
        serverpid=rt;
}

void fork_client(){
    int rt=fork();
    if(rt<0){
        alert("Unable to fork client.Exiting..");
        exit(-1);
    }
    if(rt==0){ // Start the client
        printf("Starting Client.\n");
        client();
    }
    else
        clientpid=rt;
}
