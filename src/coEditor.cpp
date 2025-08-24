#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include <iostream>
#include <chrono>
#include <thread>

coEditor::coEditor(string name, boundedBuffer& myBuff) : coEditorName(name) , beforePrint(myBuff){}

void coEditor::sendToPrint(vector<char*> &msg){
    int size = msg.size();
    for(int i = 0; i < size; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        this->beforePrint.insert(msg[i]);
    }
}