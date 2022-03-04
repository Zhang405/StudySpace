#include <pthread.h>
#include <iostream>
using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_p = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_c = PTHREAD_COND_INITIALIZER;
int count = 100;

void producer(void* arg){
    while(true){
        pthread_mutex_lock(&mutex);
        while(count>=100){
            pthread_cond_wait(&cond_p,&mutex);
        }
        count++;
        pthread_cond_broadcast(&cond_p);
        pthread_mutex_unlock(&mutex);
    }
}

void costume(void* arg){
    while(true){
        pthread_mutex_lock(mutex);
        while(count<=0){
            pthread_cond_wait(&cond_c,&mutex);
        }
        count--;
        pthread_cond_signal(&cond_p);
        pthread_mutex_unlock(mutex);
    }
}
