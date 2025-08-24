#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

class countingSemaphore {
public:
    countingSemaphore(int count);

    void up();

    void down();

    int getValue();

    void setValue(int val);

private:
    int value;
    int mySemaphores;
};

#endif  // SEMAPHORE_H
