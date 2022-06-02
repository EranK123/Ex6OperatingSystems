#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

//-------------------Q1-------------//
struct QNode {
	void* data;
	QNode* next;
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

struct Queue {
    pthread_mutex_t lock;
    pthread_cond_t cond;
	QNode *front, *rear;


    Queue createQ(){
        // pthread_mutex_lock(&lock);
        front = rear = NULL;
        // pthread_mutex_unlock(&lock);
        return *this;
    }

	void enQ(void* x)
	{
        pthread_mutex_lock(&lock);
		QNode* temp = new QNode(x);

		if (rear == NULL) {
			front = rear = temp;
			return;
		}

		rear->next = temp;
		rear = temp;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
	}


	void* deQ()
	{
        pthread_mutex_lock(&lock);
		if (front == NULL){
			pthread_cond_wait(&cond, &lock);
        }

		QNode* temp = front;
		front = front->next;
		if (front == NULL){
			rear = NULL;
        }
		void* x = malloc(sizeof(temp->data));
		memcpy(x, temp->data, sizeof(temp->data));
		delete (temp);
        pthread_mutex_unlock(&lock);
		return x;
		
	}

    void destroyQ(){
        // pthread_mutex_lock(&lock);
        if(front == NULL){
            return;
        }
        QNode *temp = front;
        while(temp){
            front = front->next;
            delete(temp);
            temp = front;
        }
        front = NULL;
        // pthread_mutex_unlock(&lock);
    }
};

//-------------------Q2-------------//

struct AO{
	Queue q;
	pthread_t pth;
};

struct AO active_object;

void newAO(Queue q, void* (*func1)(void*), void* (*func2)(void*)){
	active_object.q = q;
	void* x = active_object.q.deQ();
	active_object.q.deQ();
	pthread_create(&active_object.pth, NULL, func1, x);
	func2(x);
}

void destroyAO(AO ao){
	pthread_cancel(ao.pth);
	ao.q.destroyQ();
}


void* printElem1(void *ptr){
	cout << "Func 1 Elem is at : " << &ptr << endl;
}

void* printElem2(void *ptr){
	pthread_join(active_object.pth, NULL);
	cout << "Func 2 Elem is at: " << &ptr << endl;
}


int main()
{
	Queue q;
    q.createQ();
	int x = 5;
	int y = 8;

	q.enQ(&x);
	q.enQ(&y);
	void *f = q.deQ();
	cout << *(int*) f << endl;
	// newAO(q, &printElem1, &printElem2);
}
