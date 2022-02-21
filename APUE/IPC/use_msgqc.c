#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<string.h>

typedef struct _msg
{
    long type;
    char msg[100];
} MSG;


int main(){
    key_t key;
    int msgqid;
    key = ftok("./",2015);
    msgqid = msgget(key,IPC_CREAT|0666);
    if(msgqid == -1){
        perror("msgget");
        exit(-1);
    }
    MSG msg;
    msgrcv(msgqid,&msg,sizeof(msg)-sizeof(long),(long)10,0);
    printf("msg = %s\n",msg.msg);
    return 0;
}