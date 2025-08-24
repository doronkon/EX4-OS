#ifndef COEDITOR_H
#define COEDITOR_H

#include <string>
#include <cstring>
#include <unistd.h>
#include <queue>
#include "boundedBuffer.h"


using namespace std;

class coEditor
{
public:
    coEditor(string name,boundedBuffer &beforePrint);
    void sendToPrint(vector<char*> &msg);
    string coEditorName;
    boundedBuffer &beforePrint;
};

#endif