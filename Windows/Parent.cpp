#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <vector>

void addNewThread();
void deleteLastThread();
void deleteAllThreads();
DWORD WINAPI print(PVOID param);

std::vector<bool> closeFlags;
std::vector<HANDLE> printFlags;
HANDLE ThreadInfoArray[20];
//LPCRITICAL_SECTION criticalSectionOfPrint;

int main() {

	char command = '\0';
	//InitializeCriticalSection(criticalSectionOfPrint);

	std::cout << "Press + to add process, press - to delete last one, press q to stop program" << std::endl;

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
			deleteAllThreads();
			return 1;
		}
		}

	}
	//DeleteCriticalSection(criticalSectionOfPrint);
	return 0;
}

void addNewThread() {
	
	char EventID[50];

	sprintf_s(EventID, "%dp", closeFlags.size() + 1);
	printFlags.push_back(CreateEvent(NULL, TRUE, FALSE, EventID));
	sprintf_s(EventID, "%d", closeFlags.size() + 1);
	closeFlags.push_back(CreateEvent(NULL, FALSE, FALSE, EventID));
	char threadParam[100];
	sprintf_s(threadParam, "%s", EventID);

	HANDLE ThreadInfo = CreateThread(NULL, 0, print, (PVOID)threadParam, 0, NULL);

	ThreadInfoArray[closeFlags.size() - 1] = ThreadInfo;
}

void deleteLastThread() {

	if (closeFlags.size() > 0) {
		closeFlags.back() = true;
		WaitForSingleObject(ThreadInfoArray[closeFlags.size() - 1], INFINITE);
		CloseHandle(printFlags.back());

		closeFlags.pop_back();
		printFlags.pop_back();

	}
}

void deleteAllThreads() {
	while (closeFlags.size() > 0) {
		deleteLastThread();
	}
}

DWORD WINAPI print(PVOID _param)
{
	char* param = (char*)_param;

	while (true) {

		//EnterCriticalSection(criticalSectionOfPrint);
			for (int i = 0; param[i] != '\0'; i++) {
				
				std::cout << param[i];
				Sleep(500);
			}
			std::cout << std::endl;
		//LeaveCriticalSection(criticalSectionOfPrint);
		
	}
	return 0;
}