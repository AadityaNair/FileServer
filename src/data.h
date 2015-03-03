/*
    File Server
    Author: Aaditya M Nair (Prometheus)	
    Created On : Sunday 01 March 2015 02:15:02 PM IST

    Handles all data structuring problems.
*/

#pragma once

#include <openssl/md5.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * The data frame that will be sent.
 * TODO Upgrade to two different structures.
 * One containing metadata and other the actual data
 *
 */
#define TYPE_FILELIST_REQ 0
#define TYPE_FILEREQ      1
#define TYPE_DATA         2

struct Data{
    int datatype;
    char name[100];
    int frame_no;
    int nof;
    char md5sum[33];
    char buffer[128];
};
typedef struct Data data;

char* get_md5(char *buffer)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    char *converted=(char *)malloc((2*MD5_DIGEST_LENGTH + 1)*sizeof(char));
    MD5( (unsigned char*)buffer, strlen(buffer), result );
    
    int i;
    for(i=0;i<MD5_DIGEST_LENGTH;i++){
        sprintf(&converted[i*2], "%02X", result[i]);
    }
    return converted;
}

int get_file_size(char *filename)
{
    /*
     * Stats the filename for the size
     */
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

char* get_buffer(FILE* fd)
{
    /*
     * Gets data buffer length of characters from the file 
     * to be sent.
     * TODO Replace this with fread() 
     */
    char *buf=(char *)malloc(128*sizeof(char));
    int i; char c;
    for (i=0; i<127; i++)
    {
        c=fgetc(fd);
        buf[i]=c;
        if(c==EOF)
            break;      
    }
    buf[i]='\0';
    return buf;
}

int get_frames(char *filename, data **ptr, char *name)
{
    /*
     * Generate frames from the actual file.
     * TODO Error Handling
     */
    FILE* fd=fopen(filename, "rb");
    if (NULL == fd){
        printf("error %ld", (long)fd);
        exit(-1);
    }


    int size= get_file_size(filename);
    char buffer[128];buffer[0]='\0';

    int nof=size/127;
    if (size%127)
        nof++;

    *ptr= (data *) malloc(nof*sizeof(data));

    int i;
    for(i=0;i<nof;i++)
    {
        (*ptr+i)->frame_no=i;
        (*ptr+i)->nof=nof;
        strcpy( (*ptr+i)->name, name );
        
        strcpy(buffer, get_buffer(fd));
        strcpy( (*ptr+i)->buffer, buffer );
        strcpy( (*ptr+i)->md5sum, get_md5(buffer) );
    }
    fclose(fd);
    return 0;
}

void join_frames(data *frameset)
{
    char filename[100];
    strcpy(filename, frameset[0].name);
    FILE *fd=fopen(filename, "wb");
    int i;
    for(i=0; i<frameset[0].nof; i++)
    {
        fwrite((frameset+i)->buffer, strlen(frameset[i].buffer), 1, fd);
    }
    fclose(fd);

}
