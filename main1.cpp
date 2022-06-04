#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT 5003
using namespace std;

//-------------------Q1-------------//
struct QNode {
	void* data;
	struct QNode* next;
	int fd;
	QNode(void* d)
	{
		data = d;
		next = NULL;
	}

	// QNode(QNode &ot){
	// 	next = new QNode(ot.next);
	// 	data = ot.data;
	// }
};
pthread_mutex_t lock_;
pthread_cond_t cond_;
struct Queue {
	QNode *front, *rear;

};
Queue* createQ(){
        // pthread_mutex_lock(&lock);
		Queue *queue = (Queue *)(malloc(sizeof(struct Queue)));
		queue->rear = nullptr;
		queue->front = nullptr;
        // pthread_mutex_unlock(&lock);
        return queue;
    }


	void enQ(Queue *q ,void* x, int number_of_file_dis)
	{
        pthread_mutex_lock(&lock_);
		QNode* temp = new QNode(x);
		temp->fd = number_of_file_dis;
		if (q->rear == NULL) {
			q->front = temp;
			q->rear = temp;
			pthread_cond_signal(&cond_);
			pthread_mutex_unlock(&lock_);
			return;
		}

		q->rear->next = temp;
		q->rear = temp;
		pthread_mutex_unlock(&lock_);
        
	}


	void* deQ(Queue *q)
	{
        pthread_mutex_lock(&lock_);
		if (q->front == NULL){
			pthread_cond_wait(&cond_, &lock_);
        }

		QNode* temp = q->front;
		q->front = q->front->next;
		if (q->front == NULL){
			q->rear = NULL;
        }
		// void* x = malloc(sizeof(temp->data));
		// memcpy(x, temp->data, sizeof(temp->data));
		// delete (temp);
        pthread_mutex_unlock(&lock_);
		return temp;
		
	}

    void destroyQ(Queue *q){
        while(q->front != nullptr){
			deQ(q);
		}
		delete q;
    }


Queue *ao1q;
Queue *ao2q;
Queue *ao3q;


//-------------------Q2-------------//

typedef struct AO{
	Queue *q;
	pthread_t pth;
	void* (*f1)(void*);
	void* (*f2)(void*);
}activeo, *pactiveo;


void newAO(Queue *q, void* (*func1)(void*), void* (*func2)(void*)){
	// pactiveo new_ao = (pactiveo)(malloc(sizeof(activeo))); 
	QNode* x = (QNode *)deQ(q);
	func1(x);
	func2(x);
	// return nullptr;
}

void destroyAO(AO *ao){
	pthread_cancel(ao->pth);
	destroyQ(ao->q);
	delete ao;
}


void* printElem1(void *ptr){
	cout << "Func 1 Elem is at : " << &ptr << endl;
	return nullptr;
}

void* printElem2(void *ptr){
	cout << "Func 2 Elem is at: " << &ptr << endl;
	return nullptr;
}

void* newAO2(void *e){
	cout << 142 << endl;
	pactiveo o = (pactiveo)e;
	newAO(o->q, o->f1, o->f2);
	// return nullptr;
}

//-------------------Q3-------------//
char client_message[1024];
void* getinput(void *arg){
	// if(ao1q->front == nullptr && ao1q->rear == nullptr){
	// 	ao1q createQ();
	// }
	int newSocket = *((int *)arg);
	recv(newSocket, client_message, sizeof(client_message), 0);
	client_message[strlen(client_message)] = '\0';
	char*enq_msg = (char*)(malloc)(strlen(client_message));
	strcpy(enq_msg, client_message);
	enQ(ao1q ,enq_msg, newSocket);
	memset(client_message, 0, 1024);
	return nullptr;
}

void* ceacar(void *e){
	cout << "ceacar" << endl;
	QNode *node = (QNode *)e;
	int length = strlen((char*)node->data);
	char *s = (char*)(malloc(sizeof(node->data)));
	s = (char*)node->data;
	for(int i = 0; i < length; i++){
		if(s[i] == 'z'){
			s[i] = 'a';
		}else if(s[i] == 'a'){
			s[i] = 'z';
		}else{
			s[i] = s[i] + 1;
		}
	}
	node->data = s;
	return nullptr;
}

void *lower_upper(void* e){
	cout << "lower_upper" << endl;
	cout << 181 << endl;
	QNode *node = (QNode *)e;
	int length = strlen((char*)node->data);
	char *s = (char*)(malloc(sizeof(node->data)));
	s = (char*)node->data;
	for(int i = 0; i < length; i++){
		if(s[i] >= 65 && s[i] <= 90){
			s[i] += 32;
		}else{
			s[i] -= 32;
		}
	}
	cout << 191 << endl;
	node->data = s;
	return nullptr;
}

void* sendMessage(void *e){
	cout << 196 << endl;
	QNode *node = (QNode *)(e);
	cout << (char*)node->data << endl;
	send(node->fd, node->data, strlen((char*)node->data), 0);
	return nullptr;
}

void* run(void *e){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
	 serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET;
    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1){
        printf("Bind failed");
        return nullptr;
    }
    listen(sockfd, 4);
    while (1){
        struct sockaddr_storage taddr;
        socklen_t size_ = sizeof(taddr);
        int new_fd = accept(sockfd, (struct sockaddr *)&taddr, &size_);
        if (new_fd != -1)
        {
            pthread_t th;
            pthread_create(&th, NULL, getinput, &new_fd);
        }
    }
    return nullptr;
}

void *func1(void *arg)
{
	cout << "func1" << endl;
    QNode *n = (QNode *)arg;
    int l = strlen((char*)n->data);
    char t[l];
    strcpy(t, (char*)n->data);
    enQ(ao2q, t, n->fd);
}

void *func2(void *arg)
{	
	cout << "func2" << endl;
    struct QNode *n = (struct QNode *)arg;
    int l = strlen((char*)n->data);
    char t[l];
    strcpy(t, (char*)n->data);
    enQ(ao3q, t, n->fd);
}

typedef struct activeAos{
	pactiveo active1;
	pactiveo active2;
	pactiveo active3;
}numofaos, *pnumofaos;

int main()
{	
	pthread_t threadserver;
	pthread_t fora1;
	pthread_t fora2;
	pthread_t fora3;
	AO *a1 = (pactiveo)(malloc(sizeof(activeo)));
	AO *a2 = (pactiveo)(malloc(sizeof(activeo)));
	AO *a3 = (pactiveo)(malloc(sizeof(activeo)));

	ao1q = createQ();
	ao2q = createQ();
	ao3q = createQ();

	a3->f1 = sendMessage;
	a3->f2 = printElem1;
	a1->f1 = ceacar;
	a2->f1 = lower_upper;
	a1->f2 = func1;
	a2->f2 = func2;

	a3->q = ao3q;
	a2->q = ao2q;
	a1->q = ao1q;

	pnumofaos pnum = (pnumofaos)(malloc(sizeof(numofaos)));
	pnum->active1 = a1;
	
	pnum->active3 = a3;
	pnum->active2 = a2;

	pthread_create(&threadserver, NULL, run, ao1q);
	sleep(12);
	if(ao1q == nullptr){
		cout << "Queue is empty" << endl;
		return 0;
	}
    struct QNode *n = ao1q->front;
    while (n != NULL){
        cout << (char *)(n->data) << endl;
        n = n->next;
    }

	pthread_create(&fora1, NULL, newAO2, pnum->active1);
	sleep(5);
	pthread_create(&fora2, NULL, newAO2, pnum->active2);
	sleep(6);
	pthread_create(&fora3, NULL, newAO2, pnum->active3);
	return 0;
	// struct QNode *n1 = ao1q->front;
    // while (n1 != NULL){
    //     cout << (char *)(n1->data) << endl;
    //     n1 = n1->next;
    // }

}
