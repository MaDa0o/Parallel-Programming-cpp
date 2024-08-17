//windows specific
#include <Windows.h>
#include <strsafe.h>

#define NUM_THREAD 10
#define BUFF_SIZE 255

//arguments to be passed to the threads
struct Package{
	int tid;
	int num;
};

void display_thread(HANDLE hstdout,int tid, int num, const char* message){
	//TCHAR is windows specific char
	TCHAR message_buff[BUFF_SIZE];

	//https://stackoverflow.com/questions/1951519/when-to-use-stdsize-t
	size_t string_size;

	DWORD dwChars;

	//print message using thread safe functions
	//print a string to the buffer and check to make sure it doesn't overflows
	// https://learn.microsoft.com/en-us/windows/win32/api/strsafe/nf-strsafe-stringcchprintfa
	StringCchPrintf(message_buff,BUFF_SIZE,TEXT("printing number %d from thread%d. %s\n"),num,tid,message);
	//check for a buffer overrun
	StringCchLength(message_buff,BUFF_SIZE,&string_size);
	//write character string to stdout
	// https://learn.microsoft.com/en-us/windows/console/writeconsole
	WriteConsole(hstdout,message_buff,string_size,&dwChars,NULL);

	//put the thread to sleep for some time
	sleep(1000);

}

// DWORD is basically macros for a range similar to unsigned int (32-bit) defined in <windows.h>
// https://stackoverflow.com/questions/2995251/why-in-c-do-we-use-dword-rather-than-unsigned-int
// WINAPI is windows specific macros fro __stdcall which is a calling convention
// https://stackoverflow.com/questions/2348442/what-does-winapi-in-main-function-mean
// LPVOID is windows specific void pointer
// https://stackoverflow.com/questions/1987957/difference-between-lpvoid-and-void
DWORD WINAPI thread_function(LPVOID lpParam){
	//cast argument back to package
	Package local_package = *(Package*)lpParam;

	//make handle to get access to stdout
	HANDLE hstdout = NULL;
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	//call thread safe print method
	display_thread(hstdout,local_package.tid, local_package.num, "El psy Congroo");

	return 0;
}

int main(){
	//create array of handles, handle are like pointer, refer below link
	//https://stackoverflow.com/questions/1303123/what-is-a-handle-in-c
	HANDLE arr_handle[NUM_THREAD] = {0};

	//create array of the structure package
	Package packages[NUM_THREAD];

	//create threads
	for(int i = 0;i < NUM_THREAD; i++){
		packages[i].tid = i;
		packages[i].num = i*i;

		// Arguments:
		// 1-> Security Attributes (default is NULL)
		// 2-> stack size (default stack size represented by 0)
		// 3-> Entry Function
		// 4-> Argument (converted to void pointer)
		// 5-> Creation flag (=0 run immediately after creation)
		// 6-> pointer to variable to receive thread id
		
		arr_handle[i] = CreateThread(NULL,0,thread_function,&packages[i],0,NULL);
	}

	//wait for all the threads to be in signaled state
	//infinite states to wait forever
	WaitForMultipleObjects(NUM_THREAD,arr_handle,true,INFINITE);

	//Clean up by closing open object handles
	for(int i = 0;i<NUM_THREAD;i++){
		CloseHandle(arr_handle[i]);
	}

	return 0;
}