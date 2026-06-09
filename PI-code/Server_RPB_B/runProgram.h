#ifndef RUNPROGRAM_H
#define RUNPROGRAM_H

#include <string>
#include "TCPServer.h"
#include "CANInterface.h"

// Definitie van de verschillende soorten berichten die binnen kunnen komen via TCP.
enum class MessageType {
    BED,
    ID,
    LED,
	VENT,
    MATRIX,
    UNKNOWN
};

/**
 * @brief De runProgram klasse is verantwoordelijk voor het verwerken van binnenkomende TCP-berichten en CAN-berichten.
 * 
 */
class runProgram{
public:
    runProgram();

    void run();
    MessageType getMessageType(const std::string& key);
    void fallDetection();

    ~runProgram();

private:
    CANInterface can;

    char buffer[1024];
    void canMessageHandler(const struct can_frame& frame);
    void handleTcpMessage(const std::string& msg);
    bool bedPressure;
    uint16_t readDistance;
};

#endif
