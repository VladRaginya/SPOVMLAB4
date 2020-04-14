#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <vector>
#include <stdlib.h>

void addNewThread();
void deleteLastThread();
void deleteAllThreades();
void *threadFunc(void *args);

std::vector<pthread_t> ThreadInfoArray;
std::vector<bool *> closeFlags;
pthread_mutex_t printCS;

struct Parameters
{
    bool *closeFlag;
    int numberOfProc;
};

int main()
{
    //initscr();
    //nodelay(stdscr, TRUE);
    // noecho();
    char command = '\0';
    pthread_mutex_init(&printCS, NULL);
    std::cout << "Press + to add Thread, press - to delete last one, press q to stop program" << std::endl;
    //refresh();
    while (true)
    {
        //refresh();
        // command = getch();
        command = getchar();
        rewind(stdin);
        switch (command)
        {
        case '+':
        {
            addNewThread();
            break;
        }
        case '-':
        {
            deleteLastThread();
            break;
        }
        case 'q':
        case 'Q':
        {
            deleteAllThreades();
            pthread_mutex_destroy(&printCS);
            //endwin();
            return 1;
        }
        }
    }
}

void addNewThread()
{

    closeFlags.push_back(new bool(false));
    Parameters *params = new Parameters();
    params->closeFlag = closeFlags.back();
    params->numberOfProc = ThreadInfoArray.size() + 1;

    pthread_t ThreadInfo;
    pthread_create(&ThreadInfo, NULL, threadFunc, (void *)params);

    ThreadInfoArray.push_back(ThreadInfo);
}

void deleteLastThread()
{

    if (ThreadInfoArray.size() > 0)
    {

        *closeFlags.back() = true;
        closeFlags.pop_back();
        ThreadInfoArray.pop_back();
    }
}

void deleteAllThreades()
{
    while (ThreadInfoArray.size() > 0)
    {
        deleteLastThread();
    }
}

void *threadFunc(void *args)
{

    char ThreadID[50];
    int number = ((Parameters *)args)->numberOfProc;
    bool *isClosed = ((Parameters *)args)->closeFlag;
    sprintf(ThreadID, "%d", number);
    strcat(ThreadID, " - Number of thread\n");

    while (true)
    {
        if (*isClosed)
            return 0;
        while (pthread_mutex_trylock(&printCS) == EBUSY)
        {
        }
        for (int i = 0; ThreadID[i] != '\0'; i++)
        {
            if (*isClosed)
            {
                pthread_mutex_unlock(&printCS);
                return 0;
            }
            usleep(100000);
            printf("%c", ThreadID[i]);

            //refresh();
        }
        //refresh();
        pthread_mutex_unlock(&printCS);
    }
    return 0;
}