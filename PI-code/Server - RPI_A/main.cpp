#include "server.h"
#include <thread>

/**
 * @brief Main functie die de server start en de Wemos-handling in een aparte thread uitvoert.
 * 
 * @return int Retourneert 0 bij succesvolle uitvoering.
 */
int main() {
    startPi();

    std::thread wemosThread(initWemos);
    wemosThread.detach();

    handleWemos();

    return 0;
}
