#include "../headers/boundedBuffer.h"
#include <string>
#include <queue>
#include "../headers/countingSemaphore.h"
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <unistd.h> // for ftok

boundedBuffer::boundedBuffer(int size)
    : full(0), empty(size), semaphore(1){}

void boundedBuffer::insert(char *s)
{
    this->empty.down();
    this->semaphore.acquire();
    this->myQueue.push(s);
    this->semaphore.release();
    this->full.up();
}

char *boundedBuffer::remove()
{
    this->full.down();
    this->semaphore.acquire();
    char *s = this->myQueue.front();
    this->myQueue.pop();
    this->semaphore.release();
    this->empty.up();
    return s;
}