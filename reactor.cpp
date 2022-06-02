#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;


typedef struct Reactor{
    int fd;
    pthread_t thr;
    void* (*pointerfunc)(void*);
}*preactor, reactor;

typedef struct event{
    preactor reactor;
    int fd;
}*pevent, event;

preactor newReactor(){
    preactor react = (preactor)(malloc(sizeof(reactor)));
    return react;
}

void installHandler(preactor react, void *(*pointerfunc)(void*), int fd){
    react->fd = fd;
    react->pointerfunc = pointerfunc;
    pevent r = (pevent)malloc(sizeof(event));
    r->fd = fd;
    r->reactor = react;
    pthread_create(&react->thr, NULL, pointerfunc, r);
}

void RemoveHandler(preactor r, int fd){
    pthread_join(r->thr, NULL);
    r->fd = -1;
    r->pointerfunc = nullptr;
}

// int main(){
// return 0;
// }