#include "table.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

Table::Table(int n) {
    // TODO: implement table constructor (value, mutex, cond)
    pthread_mutex_init(&mutex, NULL);
    value = n;
    if (pthread_cond_init(&cond, NULL) != 0) {                                    
       fprintf(stderr, "Error : pthread_cond_init \n");                                                                                               
    }  
}

void Table::wait() {
    // TODO: implement semaphore wait
    pthread_mutex_lock(&mutex);
    while(value<=1){
        //printf("Now full of philosopher\n");
        pthread_cond_wait(&cond,&mutex);
    }
    value--;
    //printf("%d\n",value);
    pthread_mutex_unlock(&mutex);
}

void Table::signal() {
    // TODO: implement semaphore signal
    pthread_mutex_lock(&mutex);
    value++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

Table::~Table() {
    // TODO: implement table destructor (mutex, cond)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}