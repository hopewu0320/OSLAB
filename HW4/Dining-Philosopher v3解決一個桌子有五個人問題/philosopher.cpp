#include "philosopher.hpp"
void test();

Philosopher::Philosopher(int id, Fork *leftFork, Fork *rightFork, Table *table) :id(id), cancelled(false), leftFork(leftFork), rightFork(rightFork), table(table), priority(0) {
    srand((unsigned) time(&t1));
    
    
    
}

void Philosopher::start() {
    // TODO: start a philosopher thread
    
    if(pthread_create(&t,NULL,run,this) != 0){
        fprintf(stderr, "Error: pthread_create\n");
    }
    
}

int Philosopher::join() {
    // TODO: join a philosopher thread
    pthread_join(t,NULL);
    //printf("Now using join()\n");
}

int Philosopher::cancel() {
    // TODO: cancel a philosopher thread
    pthread_cancel(t);
    //printf("Now using cancel()\n");
}

void Philosopher::think() {
    int thinkTime = rand() % MAXTHINKTIME + MINTHINKTIME;
    sleep(thinkTime);
    printf("Philosopher %d is thinking for %d seconds.\n", id, thinkTime);
}

void Philosopher::eat() {
    printf("Philosopher %d is eating.\n", id);
    sleep(EATTIME);
}

void Philosopher::pickup(int id) {
    // TODO: implement the pickup interface, the philosopher needs to pick up the left fork first, then the right fork
    leftFork->wait();
    printf("Now %d philosopher pick up %d fork \n",id,id);
    rightFork->wait();
    printf("Now %d philosopher pick up %d fork \n",id,(id+1)%PHILOSOPHERS);
    

}

void Philosopher::putdown(int id) {
    // TODO: implement the putdown interface, the philosopher needs to put down the left fork first, then the right fork
    leftFork->signal();
    printf("Now %d philosopher put down %d fork \n",id,id);
    rightFork->signal();
    printf("Now %d philosopher put down %d fork \n",id,(id+1)%PHILOSOPHERS);
    
}

void Philosopher::enter() {
    // TODO: implement the enter interface, the philosopher needs to join the table first
    table->wait();
    printf("Now %d philosopher enters table\n",id);
}

void Philosopher::leave() {
    // TODO: implement the leave interface, the philosopher needs to let the table know that he has left
    table->signal();
    printf("Now %d philosopher leaves table\n",id); //被CS調
}


void* Philosopher::run(void* arg) {
    // TODO: complete the philosopher thread routine. 
    // Run pick up put down

    Philosopher *p = (Philosopher*) arg;
    
    
    p->enter();
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);


    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    while (!p->cancelled) {
        
        
        
        p->think();
        p->pickup(p->id);
        p->eat();
        p->putdown(p->id);
        
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        
        p->leave();
        
        p->enter();
    }
    
    
    
   
    

    return NULL;
}


void test(Philosopher*){
    printf("test\n");
}