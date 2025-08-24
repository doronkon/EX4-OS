#include "../headers/countingSemaphore.h"
#include "../headers/boundedBuffer.h"
#include <string>
#include <queue>
#include "../headers/producer.h"
#include "../headers/coEditor.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <unistd.h> // for ftok
#include <fstream>  
static int seed = 1;

countingSemaphore::countingSemaphore(int n) {
    // Increment the seed
    seed++;

    // Set the value of the semaphore
    this->value = n;

    // Create the file programmatically
    std::ofstream outFile("./myfile.txt");
    if (!outFile) {
        throw std::runtime_error("Failed to create the file");
    }

    // Generate a unique key based on the file and seed value
    key_t key1 = ftok("./myfile.txt", seed);
    if (key1 == -1) {
        perror("ftok failed");
        throw std::runtime_error("ftok failed");
    }

    // Create the semaphore set (2 semaphores)
    this->mySemaphores = semget(key1, 2, IPC_CREAT | 0666); // Use appropriate permissions (e.g., 0666)
    if (this->mySemaphores == -1) {
        perror("semget failed");
        throw std::runtime_error("semget failed");
    }

    // Initialize the first semaphore (s1) to 1
    if (semctl(this->mySemaphores, 0, SETVAL, 1) == -1) {
        perror("Failed to initialize s1");
        throw std::runtime_error("Failed to initialize s1");
    }

    // Calculate the minimum value for the second semaphore (s2)
    int findMin = (this->value < 1) ? this->value : 1;

    // Initialize the second semaphore (s2) with the calculated value
    if (semctl(this->mySemaphores, 1, SETVAL, findMin) == -1) {
        perror("Failed to initialize s2");
        throw std::runtime_error("Failed to initialize s2");
    }

    // Clean up: delete the file after use
    if (std::remove("./myfile.txt") != 0) {
        throw std::runtime_error("Failed to delete the file");
    }
}


void countingSemaphore::down()
{
    struct sembuf op[2];

    op[0].sem_num = 1;
    op[0].sem_op = -1;
    op[0].sem_flg = 0;

    op[1].sem_num = 0;
    op[1].sem_op = -1;
    op[1].sem_flg = 0;

    if (semop(this->mySemaphores, op, 2) == -1)
    {
        throw std::runtime_error("Failed to decrement semaphores");
    }
    this->value--;
    if (this->value > 0)
    {
        struct sembuf incS2;
        incS2.sem_num = 1;
        incS2.sem_op = 1;
        incS2.sem_flg = 0;
        if (semop(this->mySemaphores, &incS2, 1) == -1)
        {
            throw std::runtime_error("Failed to increase s2");
        }
    }
    struct sembuf incS1;
    incS1.sem_num = 0;
    incS1.sem_op = 1;
    incS1.sem_flg = 0;
    if (semop(this->mySemaphores, &incS1, 1) == -1)
    {
        throw std::runtime_error("Failed to increase s1");
    }
}

void countingSemaphore::up() {
    struct sembuf decS1;

    decS1.sem_num = 0;
    decS1.sem_op = -1;
    decS1.sem_flg = 0;
    if (semop(this->mySemaphores, &decS1, 1) == -1)
    {
        throw std::runtime_error("Failed to decrese s1");
    }
    this->value++;
    if (this->value == 1)
    {
        struct sembuf incS2;

        incS2.sem_num = 1;
        incS2.sem_op = 1;
        incS2.sem_flg = 0;
        if (semop(this->mySemaphores, &incS2, 1) == -1)
        {
            throw std::runtime_error("Failed to increase s2");
        }
    }
    struct sembuf incS1;
    incS1.sem_num = 0;
    incS1.sem_op = 1;
    incS1.sem_flg = 0;
    if (semop(this->mySemaphores, &incS1, 1) == -1)
    {
        throw std::runtime_error("Failed to increase s1");
    }
}