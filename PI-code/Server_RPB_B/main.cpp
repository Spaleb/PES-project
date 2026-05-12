#include "runProgram.h"

/**
 * @brief Small main code since all the classes are split up. It starts
 * the runProgram class which sets up bots sockets (TCP & CAN).
 * 
 * @return int 
 */
int main() {
    runProgram app;
    app.run();
}
