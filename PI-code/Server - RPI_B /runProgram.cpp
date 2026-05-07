#include "runProgram.h"

#include <iostream>
#include <sys/select.h>
#include <algorithm>
#include <cstdint>

/**
 * @brief Construct a new run Program::run Program object
 * 
 */
runProgram::runProgram() : can("can0"){}

/**
 * @brief Runs the main program. Lets client connect and reads TCP and CAN that have been send.
 * 
 */
void runProgram::run(){
    tcp.start(8080);
    can.open();

    int client = -1;

    std::cout << "Server started\n";

    while (true) {

        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(tcp.getServerFd(), &readfds);
        FD_SET(can.getFd(), &readfds);

        if (client != -1)
            FD_SET(client, &readfds);

        int maxfd = std::max({tcp.getServerFd(), can.getFd(), client});

        select(maxfd + 1, &readfds, nullptr, nullptr, nullptr);

        // new client
        if (FD_ISSET(tcp.getServerFd(), &readfds)) {
            client = tcp.acceptClient();
            std::cout << "Client connected\n";
        }

        // TCP
        if (client != -1 && FD_ISSET(client, &readfds)) {
            int len = tcp.readClient(buffer, sizeof(buffer) - 1);

            if (len <= 0) {
                std::cout << "Client disconnected\n";
                client = -1;
            } else {
                buffer[len] = '\0';
                std::cout << "TCP: " << buffer << "\n";
            }
        }

        // CAN
        if (FD_ISSET(can.getFd(), &readfds)) {
            struct can_frame frame;

            if (can.readFrame(frame)) {
                canMessageHandler(frame);
            }
        }
    }

}

/**
 * @brief Needed for reading CAN frames. Another Header_ID means that a frame must be read differently. 
 * This function will be extended in the future. 
 * 
 * @param frame 
 */
void runProgram::canMessageHandler(const struct can_frame& frame){
    switch (frame.can_id & CAN_SFF_MASK) {

        case 0x102: {
            uint16_t distance =
                (frame.data[0] << 8) | frame.data[1];

            std::cout << "Distance: " << distance << " cm\n";
            tcp.sendClient("Distance received\n");
            break;
        }

        default:
            std::cout << "Unknown CAN ID\n";
            break;
    }
}

/**
 * @brief Destroy the run Program::run Program object
 * 
 */
runProgram::~runProgram(){

}
