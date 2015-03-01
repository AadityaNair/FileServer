/*
    File Server
    Author: Aaditya M Nair (Prometheus)
    Created On : Saturday 28 February 2015 11:41:03 PM IST

    This Code generates file list that is sent to the peer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILELIST_NAME "filelist"

#include "logger.h"

int main(int argc, char *argv[]) 
{
    FILE* list_descriptor;
    list_descriptor = fopen(FILELIST_NAME, "a");

    if( argc ==  1 )
        alert("No Directory Specified.");
    else{
        
        int i;
        for(i=1; i<argc; i++){
            fprintf(list_descriptor, "%s\n", argv[i]);
            success(argv[i]);
        }
    }

    fclose( list_descriptor );
    return 0;
}
