#include "../headers/dispatcher.h"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include "../headers/producer.h"
#include "../headers/coEditor.h"

using namespace std;
dispatcher::dispatcher(vector<producer> &producers) : myProducers(producers) {}

void dispatcher::sortMsg()
{
    int amountOfDone = 0;
    int size = this->myProducers.size();
    while (amountOfDone != size)
    {
        for (int i = 0; i < size; i++)
        {
            if (!this->myProducers[i].mybuf.myQueue.empty())
            {
                char *current = this->myProducers[i].mybuf.remove();
                if (strcmp(current, "DONE") != 0)
                {
                    if (strstr(current, "NEWS") != NULL)
                    {
                        this->news.push_back(current);
                    }
                    else if (strstr(current, "SPORTS") != NULL)
                    {
                        this->sport.push_back(current);
                    }
                    else
                    {
                        this->weather.push_back(current);
                    }
                }
                else if (strcmp(current, "DONE") == 0)
                {
                    amountOfDone++;
                }
            }
        }
    }
    this->news.push_back((char *)"DONE");
    this->sport.push_back((char *)"DONE");
    this->weather.push_back((char *)"DONE");
}