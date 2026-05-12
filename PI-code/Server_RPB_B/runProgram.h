#ifndef RUNPROGRAM_H
#define RUNPROGRAM_H

#include "TCPServer.h"
#include "CANInterface.h"

class runProgram{
public:
    runProgram();

    void run();

    ~runProgram();

private:
    TCPServer tcp;
    CANInterface can;

    char buffer[1024];
    void canMessageHandler(const struct can_frame& frame);
};

#endif
