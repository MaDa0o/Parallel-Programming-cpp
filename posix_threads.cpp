// posix thread is a thread implementation library commonly used in unix systems
// https://www.geeksforgeeks.org/posix-threads-in-os/
// posix thread runs on many platforms and has less abstraction than thread library
// https://stackoverflow.com/questions/13134186/c11-stdthread-vs-posix-threads

#include <iostream>
#include <pthread.h>

using namespace std;

#define NUM_THREAD 10

//global pthread implementation of mutex lock
pthread_mutex_t mtx;

struct Args
{
	int tid;
	int num;
};

//The entry function must have a void* return type and a single void* argument that we will have the type cast
void *thread_entry_function(void *arg){
	//typecast back the void pointer to the struct type
	Args *thread_arg = (Args*)arg;
	//critical section ahead get the mutex lock before entering it
	pthread_mutex_lock(&mtx);
	cout<<"printing from thread no. "<<thread_arg->tid<<endl;
	cout<<"having data: "<<thread_arg->num<<endl;
	//releasing the lock else there will be deadlock
	pthread_mutex_unlock(&mtx);
}

int main(){
	//Array of threads, pthread_t is the data type for the threads
	pthread_t threads[NUM_THREAD];
	//arguments for each thread
	Args arg_threads[NUM_THREAD];

	//create the threads
	for(int i = 0;i<NUM_THREAD;i++){
		//populate the arguments
		arg_threads[i].tid = i;
		arg_threads[i].num = i*i;

		//launch the thread
		// 1) pointer to thread
		// 2) Attributes for the thread (NULL means default)
		// 3) Entry routine
		// 4) Arguments
		pthread_create(&threads[i],NULL,thread_entry_function, (void*)&arg_threads[i]);
	}

	//now wait for all thread to finish executing before executing the function

	//return code for the thread to check if the thread exited successfully or not
	void *ret;

	for(int i = 0;i<NUM_THREAD;i++){
		pthread_join(threads[i], &ret);
	}

	return 0;
}