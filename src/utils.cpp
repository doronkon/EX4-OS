#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>
#include "../headers/utils.h"
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include "../headers/boundedBuffer.h"
#include "../headers/countingSemaphore.h"
#include "../headers/dispatcher.h"
using namespace std;

vector<string> splitBySpace(const string &line)
{
    vector<string> words;
    istringstream stream(line);
    string word;
    while (stream >> word)
    {
        words.push_back(word);
    }
    return words;
}

void finalPrint(boundedBuffer &beforePrint)
{
    int amountOfDone = 0;
    while(amountOfDone != 3){
        if (!beforePrint.myQueue.empty()){
            char *curr = beforePrint.remove();
            if(strcmp(curr,"DONE") == 0){
                amountOfDone++;
            } else {
                cout << curr << endl;
            }
        }
    }
    cout << "DONE" << endl;
}

void createThreads(vector<producer> &producers, vector<coEditor> &coEditors)
{
    dispatcher ourDispatcher(producers);
    vector<std::thread> threads;

    std::thread dispatcherThread(&dispatcher::sortMsg, &ourDispatcher);

    // Corrected producer loop to pass by reference
    for (auto& prod : ourDispatcher.myProducers) {
        threads.push_back(std::thread(&producer::insertMyMsg, &prod)); // pass each producer to the thread
    }

    // Join the dispatcher thread
    if(dispatcherThread.joinable())
    {
        dispatcherThread.join();
    }

    // Join all producer threads
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    // Add coEditor and finalPrint threads
    threads.push_back(std::thread(&coEditor::sendToPrint, coEditors[0], std::ref(ourDispatcher.news)));
    threads.push_back(std::thread(&coEditor::sendToPrint, coEditors[1], std::ref(ourDispatcher.sport)));
    threads.push_back(std::thread(&coEditor::sendToPrint, coEditors[2], std::ref(ourDispatcher.weather)));
    threads.push_back(std::thread(finalPrint, std::ref(coEditors[0].beforePrint)));

    // Join all remaining threads
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}


void createProducers(const char *configFileName)
{
    vector<producer> producers;
    vector<coEditor> coEditors;
    // Open the file to check if it exists and can be read
    ifstream configFile(configFileName);
    if (!configFile)
    {
        cerr << "Error: Could not open file " << configFileName << endl;
        exit(1);
    }
    string line;
    vector<string> afterSplit;
    while (getline(configFile, line))
    {
        if (line.empty())
        {
            continue;
        }
        afterSplit = splitBySpace(line);
        if (afterSplit[0] == "PRODUCER")
        {

            // getting the producer ID
            int producerId = stoi(afterSplit[1]);

            // getting the amount of messages that producer can produce
            getline(configFile, line);
            int amountOfMessages = stoi(line);

            // getting the queue size of that producer
            getline(configFile, line);
            afterSplit = splitBySpace(line);
            int queueSize = stoi(afterSplit[3]);
            // creating a producers and their buffers
            boundedBuffer newBuffer(queueSize);
            producer newProducer(producerId, amountOfMessages, newBuffer);
            producers.push_back(newProducer);
            // test(producers,coEditors);
        }
        else if (afterSplit[0] == "Co-Editor")
        {
            // create a new co-editor
            int coEditorSize = stoi(afterSplit[4]);
            boundedBuffer beforePrint(coEditorSize);
            coEditor newsCoEditor("news coEditor", beforePrint);
            coEditor sportCoEditor("sport coEditor", beforePrint);
            coEditor weatherCoEditor("weather coEditor", beforePrint);
            coEditors.push_back(newsCoEditor);
            coEditors.push_back(sportCoEditor);
            coEditors.push_back(weatherCoEditor);
        }
    }
    createThreads(producers, coEditors);
}
