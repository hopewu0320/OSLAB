#include "fork.hpp"
#include <cstdio>

Fork::Fork() {
    // TODO: implement fork constructor (value, mutex, cond)
    
    pthread_mutex_init(&mutex, NULL);
    //value = 0;
    avaliable = true;
    if (pthread_cond_init(&cond, NULL) != 0) {                                    
       fprintf(stderr, "Error : pthread_cond_init \n");                                                                                               
    }  
    
}

void Fork::wait() {
    // TODO: implement semaphore wait
    pthread_mutex_lock(&mutex);
    while(avaliable!=true){  //if not avaliable
        pthread_cond_wait(&cond,&mutex);
    }
    avaliable = false;
    pthread_mutex_unlock(&mutex);
    
}

void Fork::signal() {
    // TODO: implement semaphore signal
    pthread_mutex_lock(&mutex);
    avaliable = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

Fork::~Fork() {
    // TODO: implement fork destructor (mutex, cond)
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
}