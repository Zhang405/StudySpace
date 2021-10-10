#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define TIMESTRSIZE 1024
int main()
{
    time_t timeStamp = time(NULL);

    struct tm* tm = localtime(&timeStamp);
    tm->tm_mday+=100;

    //mktime会自动调整不合理的时间，比如天数溢出会发生进位等
    (void)mktime(tm);

    char timestr[TIMESTRSIZE];
    strftime(timestr,TIMESTRSIZE,"100days later:%Y - %m - %d",tm);
    puts(timestr);
    exit(1);
}