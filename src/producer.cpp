#include <random>
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include <string>
#include <cstring>  
#include <iostream>

producer::producer(int id, int amount, boundedBuffer& myBuff) : myId(id), amountOfMsg(amount), mybuf(myBuff){}

char* producer::createRandomMsg()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 30);
    int rand = dis(gen) % 3;  

    std::string msg;  
    if (rand == 0) {
        msg = "producer " + std::to_string(this->myId) + " NEWS " + std::to_string(this->amountNews++);
    } else if (rand == 1) {
        msg = "producer " + std::to_string(this->myId) + " SPORTS " + std::to_string(this->amountSport++);
    } else {
        msg = "producer " + std::to_string(this->myId) + " WEATHER " + std::to_string(this->amountWeather++);
    }

    // Convert std::string to C-style string
    char* cstr = new char[msg.length() + 1];  
    std::strcpy(cstr, msg.c_str());           

    return cstr;
}

void  producer::insertMyMsg(){
    for(int i = 0; i < this->amountOfMsg; i++){
        char* randomMsg = createRandomMsg();
        this->mybuf.insert(randomMsg);
    }
    this->mybuf.insert((char*)"DONE");
}