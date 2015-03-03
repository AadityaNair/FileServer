/*
    File Server
    Author: Aaditya M Nair (Prometheus)	
    Created On : Sunday 01 March 2015 08:09:35 PM IST

    The main server code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "logger.h"
#include "data.h"
#define PORT 6494
int listenSocket, connSocket;

int start_conn(){
    struct sockaddr_in server_addr;
    listenSocket=connSocket=0; 
    
    listenSocket= socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket<0){
        alert("[SERVER]  Error creating a socket.");
        return 0;
    }
    else{
        success("[SERVER]  Socket Creation Successful.");
    }
    memset((void*)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    server_addr.sin_port=htons(PORT);

    int ret=bind(listenSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret<0){
        alert("[SERVER]  Error while binding the socket.");
        return -1;
    }
    else{
        success("[SERVER]  Successfully bound the socket.");
    }
    if(listen(listenSocket, 10)==-1)
        alert("[SERVER]  Failed to establish listening.");
    warning("[SERVER]  Waiting for client to connect....");

    while((connSocket=accept(listenSocket , (struct sockaddr*)NULL,NULL))<0);
    success("[SERVER]  CONNECTED");

   return 0;
}

void stop_conn()
{
    printf("Server Stop\n");
    close(listenSocket);
    close(connSocket);
}

int server()
{
    start_conn();
    data buffer;
    printf("SERVER STARTED\n");
    while(1)
    {
        memset((void*)&buffer, 0, sizeof(buffer));
        int ret;
        ret=recv(connSocket, (void *)&buffer, sizeof(buffer), 0);
        if(ret>0)
        {
            printf("RECIEVED\n ");
            if(buffer.datatype == TYPE_FILELIST_REQ)
            {
                data *frame=NULL;
                get_frames("filelist", &frame, "list");
                int i, nof=frame[0].nof;

                for(i=0;i<nof;i++)
                {
                    ret=send(connSocket, (const void*)(frame+i), sizeof(frame[i]), 0);
                }
            }
        }
        while((connSocket=accept(listenSocket , (struct sockaddr*)NULL,NULL))<0);
    }
    stop_conn();
    return 0;
}
