#include "server.h"
#include <thread>

int main() {
    startPi();

    std::thread wemosThread(initWemos);
    wemosThread.detach();

    handleWemos();

    return 0;
}
