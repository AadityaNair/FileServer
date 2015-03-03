/*
    FileServer
    Author: Aaditya M Nair (Prometheus)	
    Created On : Monday 02 March 2015 01:00:20 AM IST

    The Main Client Code.
*/

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "logger.h"
#include "data.h"
int ClientSocket;

void start_client()
{
    ClientSocket = 0;
	struct sockaddr_in serv_addr;
	ClientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(ClientSocket<0)
    {
		alert("[CLIENT]  ERROR WHILE CREATING A SOCKET");
		return;
	}
	else
		success("[CLIENT]  Socket created");

	int portno = 6494;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	while(connect(ClientSocket,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0);
}
void stop_client()
{
    close(ClientSocket);
}

int client()
{
    start_client();
    data buffer, *frameset;
    memset((void*)&buffer, 0, sizeof(buffer));

    buffer.datatype=TYPE_FILELIST_REQ;
    send(ClientSocket, (const void *)&buffer, sizeof(buffer), 0);
    memset((void*)&buffer, 0, sizeof(buffer));
    while(1)
    {
        int ret;
        ret=recv(ClientSocket, (void *)&buffer, sizeof(buffer), 0);
        if(ret>0)
        {
            int nof=buffer.nof, i;
            frameset=(data*)malloc(nof*sizeof(data));
            frameset[0]=buffer;

            for(i=1;i<nof;i++)
            {
                ret=recv(ClientSocket, (void *)&buffer, sizeof(buffer), 0);
                frameset[i]=buffer;
            }
           join_frames(frameset); 
        }
    }
    stop_client();
    exit(0);
    return 0;
}
