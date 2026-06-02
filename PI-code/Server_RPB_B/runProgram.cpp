#include "runProgram.h"

#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <algorithm>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <cmath>
#include <chrono>
#include <map>

std::map<std::string, std::string> rfidNames = {
    {"D935D814", "Beheerder"},
    {"67B37A05", "Bezoeker"},
    {"B155721D", "Verzorger Jan"},
    {"F4889C04", "Arts Marko"}
};

TCPServer serverIn;   // 8080 → Pi A
TCPServer serverOut;  // 8081 → Qt/nc

std::map<std::string, bool> rfidPresent; // true = aanwezig, false = weg

enum CAN_ID
{
    DEUR = 0x230
};

static std::string trim(const std::string& value) {
    const char* whitespace = " \t\n\r";
    size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

static void parseKeyValue(const std::string& input, std::string& key, std::string& value) {
    std::string buffer = trim(input);

    if (!buffer.empty() && buffer.front() == '{' && buffer.back() == '}') {
        buffer = trim(buffer.substr(1, buffer.size() - 2));
    }

    size_t colon = buffer.find(':');
    if (colon == std::string::npos) {
        colon = buffer.find('=');
    }

    if (colon != std::string::npos) {
        key = trim(buffer.substr(0, colon));
        value = trim(buffer.substr(colon + 1));

        if (!key.empty() && key.front() == '"' && key.back() == '"') {
            key = key.substr(1, key.size() - 2);
        }
        if (!value.empty() && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
    }
}

runProgram::runProgram() : can("can0"){}
runProgram::~runProgram() {}

void runProgram::run(){ 
    serverIn.start(8080);
    serverOut.start(8081);
    can.open();

    int clientA = -1;
    int clientB = -1;

    std::cout << "Server started (8080=IN, 8081=OUT)\n";

    char buffer[1024];

    while (true) {

        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(serverIn.getServerFd(), &readfds);
        FD_SET(serverOut.getServerFd(), &readfds);

        if (clientA != -1)
            FD_SET(clientA, &readfds);

        int maxfd = std::max({serverIn.getServerFd(), 
                              serverOut.getServerFd(), 
                              clientA});

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10 ms                      

        if (select(maxfd + 1, &readfds, nullptr, nullptr, &timeout) < 0) {
            perror("select");
            continue;
        }

        // ✅ Pi A connect (PORT 8080)
        if (FD_ISSET(serverIn.getServerFd(), &readfds)) {
            clientA = serverIn.acceptClient();
            std::cout << "Pi A connected\n";
        }

        // ✅ Qt/nc connect (PORT 8081)
        if (FD_ISSET(serverOut.getServerFd(), &readfds)) {
            clientB = serverOut.acceptClient();
            std::cout << "Qt/nc connected\n";
        }

        // ✅ Data van Pi A → doorsturen
        if (clientA != -1 && FD_ISSET(clientA, &readfds)) {

            int len = recv(clientA, buffer, sizeof(buffer), 0);

            if (len <= 0) {
                std::cout << "Pi A disconnected\n";
                close(clientA);
                clientA = -1;
                continue;
            }

            std::string incoming(buffer, len);
            std::cout << incoming << std::endl;

            handleTcpMessage(incoming);

            // 🔥 FORWARD naar Qt/nc
            if (clientB != -1) {
                int sent = send(clientB, buffer, len, 0);
                if (sent <= 0) {
                    std::cout << "Qt/nc disconnected\n";
                    close(clientB);
                    clientB = -1;
                }
            }
        }

        // ✅ CAN → ook naar Qt/nc
        if (FD_ISSET(can.getFd(), &readfds)) {
            struct can_frame frame;

            if (can.readFrame(frame)) {
                canMessageHandler(frame);

                if (clientB != -1) {
                    send(clientB, "Distance received\n", 18, 0);
                }
            }
        }

        fallDetection();
    }

  

}

void runProgram::canMessageHandler(const struct can_frame& frame){
    switch (frame.can_id & CAN_SFF_MASK) {

        case 0x102: {
            uint16_t distance =
                (frame.data[0] << 8) | frame.data[1];

            std::cout << "Distance: " << distance << " cm\n";
            readDistance = distance;
            break;
        }

        default:
            std::cout << "Unknown CAN ID\n";
            break;
    }
}

MessageType runProgram::getMessageType(const std::string& key)
{
    if (key == "BED"){
        return MessageType::BED;
    }
    if (key == "ID"){
        return MessageType::ID;
    }

     return MessageType::UNKNOWN;
}

void runProgram::handleTcpMessage(const std::string& msg) {
    std::string key;
    std::string value;
    parseKeyValue(msg, key, value);

    if (key.empty()) {
        std::cout << "TCP handler: no valid key/value found\n";
        return;
    }

    switch (getMessageType(key)){
        case MessageType::BED:
            if (value == "ON"){
                std::cout << "BED turned ON\n";
                bedPressure = true;
            }else {
                std::cout << "BED turned OFF\n";
                bedPressure = false;
            }

        case MessageType::ID:
        {
            bool isNowPresent = !rfidPresent[value]; // toggle
                rfidPresent[value] = isNowPresent;

                std::string name = rfidNames.count(value) ? rfidNames[value] : "Onbekend (" + value + ")";
                    std::string status = isNowPresent ? "aanwezig" : "vertrokken";

            if (value == "D935D814"){ // Beheerder
                if (isNowPresent) {
            can.sendCAN(DEUR, {0x01});
            // DASHBOARD AAN/OPEN
                }
         } else if (value == "67B37A05") { // Bezoeker
        if (isNowPresent) {
        }
    } else if (value == "B155721D") {
        serverIn.sendClient("B:" + name + " " + status);
    } else if (value == "F4889C04") {
        serverIn.sendClient("B:" + name + " " + status);
    }

    break;
}
}
}


void runProgram::fallDetection()
{
    static bool prevBedPressure = false;
    static uint16_t prevReadDistance = 0;
    static std::time_t fallTime = 0;

    std::time_t now = std::time(nullptr);

    bool justLeftBed = (prevBedPressure && !bedPressure);

    if (justLeftBed)
    {
        prevReadDistance = readDistance;
        fallTime = now;
        std::cout << "JUSTLEFTBED TRUE!!" << std::endl;
        std::cout << prevReadDistance << std::endl;
    }

    double dt = std::difftime(now, fallTime);

    if (bedPressure && readDistance >= 20 && readDistance <= 50)
    {
        // in bed (send to dashboard)
    }

    else if (!bedPressure)
    {
        // out of bed (send to dashboard)
    }

    /**
     * If there is no pressure anymore and the delta distance is greater
     * than 30cm it means somebody fell out of their bed.
     */
    if (justLeftBed)
    {
        if (dt <= 1 &&
            std::abs((int)readDistance - (int)prevReadDistance) > 30)
        {
            std::cout << "FALL DETECTED!!" << std::endl;
            // SENT CAN MESSAGE FOR ALARM
        }
    }

    prevBedPressure = bedPressure;
}
