#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <sys/mman.h>
using namespace std;
pthread_mutex_t lock_;
template <typename T>   
class Singelton{
    private:
    inline static Singelton<T>* singel = NULL;
    Singelton(){}
    ~Singelton(){
        singel = NULL;
    };
    public:
    static Singelton<T> *Instance(){
        pthread_mutex_lock(&lock_);
        if(singel == NULL){
            singel = new Singelton<T>;
        }
        return singel;
    }
    static void Destroy(){
        pthread_mutex_lock(&lock_);
        delete singel;
        singel = NULL;
        pthread_mutex_unlock(&lock_);
    }
 
    void print(){
        cout << this << endl;
    }
};

int main()
{
    Singelton<int>* s = Singelton<int>::Instance();
    void* p = mmap (s, 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    cout<< s << endl;
    cout << p;
}
