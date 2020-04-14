#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <process.h>

void addNewThread();
void deleteLastThread();
void deleteAllThreades();
unsigned int WINAPI threadFunc(PVOID args);

std::vector<HANDLE> ThreadInfoArray;
std::vector<bool*> closeFlags;
CRITICAL_SECTION printCS;
struct Parameters {
	bool* closeFlag;
	int numberOfProc;
};

int main() {

	char command = '\0';
	InitializeCriticalSection(&printCS);
	std::cout << "Press + to add Thread, press - to delete last one, press q to stop program" << std::endl;

	while (true) {

		while (!_kbhit());
		command = _getch();
		rewind(stdin);
		switch (command) {
		case '+': {
			addNewThread();
			break;
		}
		case '-': {
			deleteLastThread();
			break;
		}
		case 'q':
		case 'Q': {
			deleteAllThreades();
			DeleteCriticalSection(&printCS);
			return 1;
		}
		}
	}
}

void addNewThread() {

	closeFlags.push_back(new bool(false));
	Parameters* params = new Parameters();
	params->closeFlag = closeFlags.back();
	params->numberOfProc = ThreadInfoArray.size() + 1;

	HANDLE ThreadInfo = (HANDLE)_beginthreadex(NULL, NULL, threadFunc, params, NULL, NULL);

	ThreadInfoArray.push_back(ThreadInfo);
}

void deleteLastThread() {

	if (ThreadInfoArray.size() > 0) {
		
		*closeFlags.back() = true;
		closeFlags.pop_back();
		ThreadInfoArray.pop_back();

	}
}

void deleteAllThreades() {
	while (ThreadInfoArray.size() > 0) {
		deleteLastThread();
	}
}

unsigned int WINAPI threadFunc(PVOID args) {
	
	char ThreadID[50];
	int number = ((Parameters*)args)->numberOfProc;
	bool* isClosed = ((Parameters*)args)->closeFlag;
	_itoa(number, ThreadID, 10);
	strcat(ThreadID, " - Number of thread");

	while (true) {
			if (*isClosed) return 0;
			while(!TryEnterCriticalSection(&printCS));
			for (int i = 0; ThreadID[i] != '\0'; i++) {
				if (*isClosed) {
					LeaveCriticalSection(&printCS);
					return 0;
				}
				printf("%c", ThreadID[i]);
				Sleep(100);
			}
			std::cout << std::endl;
			LeaveCriticalSection(&printCS);
			Sleep(1);
	}
	return 0;
}
