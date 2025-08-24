#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <string>
#include <cstring>
#include <unistd.h>
#include <queue>
#include "producer.h"

using namespace std;

class dispatcher
{
public:
    dispatcher(vector<producer> &producers);
    void sortMsg();
    vector<producer> &myProducers;
    vector<char*> news;
    vector<char*> sport;
    vector<char*> weather;
};

#endif