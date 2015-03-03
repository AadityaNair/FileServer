/*
    File Server
    Author: Aaditya M Nair (Prometheus)	
    Created On : Saturday 28 February 2015 11:58:12 PM IST

    This program is used to print coloured output.
    Generally used for logging
*/
#pragma once
#include <stdio.h>

#define KNRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define SERVER 1
#define CLIENT 2
/*
 * The colours are used as shown below.
    int main()
    {
      printf(RED "red\n" RESET);
      printf(GREEN "green\n" RESET);
      printf(YELLOW "yellow\n" RESET);
      printf(BLUE "blue\n" RESET);
      printf(KMAG "magenta\n" RESET);
      printf(KCYN "cyan\n" RESET);
      printf(KWHT "white\n" RESET);
      printf(KNRM "normal\n" RESET);

      return 0;
    }
*/


void warning(char *s)
{
       printf(YELLOW "%s\n" RESET, s);
}

void alert(char *s)
{
       printf(RED "%s\n" RESET, s);
}

void success(char *s)
{
       printf(GREEN "%s\n" RESET, s);
}
