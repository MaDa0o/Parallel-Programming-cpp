#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

//mutex lock to be acquired by the critical section before doing operation
mutex mtx;

//initial function for threads
void call_from(int tid){
	mtx.lock();
	//critical section
	cout<<"call from thread "<<tid<<"\n";
	mtx.unlock();
}

int main(){
	thread t[10];
	for(int i = 0;i<10;i++){
		t[i] = thread(call_from, i);
	}

	mtx.lock();
	cout<<"call from thread main\n";
	mtx.unlock();

	//wait for threads to finish

	for(int i = 0;i<10;i++){
		t[i].join();
	}

	return 0;
}

//now the tread acquires a mutex lock in it's critical section before accessing cout, therefore the results are not intermingled
//but we can still not control in which order does the threads access the critical section, hence the out of order result
