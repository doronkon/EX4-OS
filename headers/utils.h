#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include "boundedBuffer.h"

// Splits a given string by spaces and returns a vector of words
std::vector<std::string> splitBySpace(const std::string& line);

void createProducers(const char* configFileName);
void createThreads(std::vector<producer> &producers, std::vector<coEditor> &coEditors);
void test(vector<producer> &producers, vector<coEditor> &coEditors);

#endif // UTILS_H
