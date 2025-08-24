#include <iostream>
#include <fstream>
#include "../headers/utils.h"
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include <vector>

int main(int argc, char* argv[]) {
    // Check if the required argument is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    // Access the second argument (config.txt in this case)
    const char* configFileName = argv[1];
    createProducers(configFileName);
}
