#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include <string>
#include <cstring>
#include <unistd.h>
#include <queue>
#include "countingSemaphore.h"
#include <mutex>
#include <semaphore>

using namespace std;

class boundedBuffer
{
public:
    boundedBuffer(int size);
    void insert (char* s);
    char* remove ();
    bool emptyQueue();
    void up();
    void down();
    int size;
    queue<char*> myQueue;
    countingSemaphore full;
    countingSemaphore empty;
    //mutex mtx;
    std::binary_semaphore semaphore;
};

#endif