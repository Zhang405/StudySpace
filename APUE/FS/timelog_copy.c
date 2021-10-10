#include <stdlib.h>
#include <stdio.h>

#define _FILE_PATH "./log.txt"

int main()
{
    FILE* pLogFile = fopen(_FILE_PATH,"a+");
    if(pLogFile == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    
}
