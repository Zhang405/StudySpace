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
    msg.type = 10;
    strcpy(msg.msg,"hello zmj");
    msgsnd(msgqid,&msg,sizeof(msg)-sizeof(long),0);
    return 0;
}