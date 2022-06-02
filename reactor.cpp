#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;


typedef struct Reactor{
    pthread_t thr;
    void* (*pointerfunc)(void*);
    int fd;
}*preactor, reactor;

typedef struct event{
    preactor reactor;
    int fd;
}*pevent, event;

preactor newReactor(){
    preactor react = (preactor)(malloc(sizeof(reactor)));
    return react;
}

void installHandler(preactor react, void *(pointerfunc)(void*), int fd){
    react->fd = fd;
    react->pointerfunc = pointerfunc;
    pevent eve = (pevent)malloc(sizeof(event));
    eve->fd = fd;
    eve->reactor = react;
    pthread_create(&react->thr, NULL, pointerfunc, eve);
}

void RemoveHandler(preactor r, int fd){
    pthread_join(r->thr, NULL);
    r->fd = -1;
    r->pointerfunc = nullptr;
}

// int main(){
// return 0;
// }