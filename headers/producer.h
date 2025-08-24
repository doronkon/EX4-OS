#ifndef PRODUCER_H
#define PRODUCER_H

#include <string>
#include <cstring>
#include <unistd.h>
#include <queue>
#include "boundedBuffer.h"
#include "producer.h"

using namespace std;

class producer
{
public:
    producer(int id, int amount,boundedBuffer &myBuff);
    char* createRandomMsg();
    void insertMyMsg();
    int myId;
    int amountOfMsg;
    boundedBuffer &mybuf;
    int amountNews = 0;
    int amountSport = 0;
    int amountWeather = 0;
};

#endif