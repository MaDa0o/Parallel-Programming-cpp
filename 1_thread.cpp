#include <iostream>
#include <thread>
using namespace std;

void call_from(int tid){
	cout<<"call from thread "<<tid<<"\n";
}

int main(){
	thread t[10];
	for(int i = 0;i<10;i++){
		t[i] = thread(call_from, i);
	}

	cout<<"call from thread main\n";

	//wait for threads to finish

	for(int i = 0;i<10;i++){
		t[i].join();
	}

	return 0;
}

//cout is not thread safe so when multiple threads try to access cout at the same time it would lead to jumbled output