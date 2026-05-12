#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <string>
#include <linux/can.h>

/**
 * @brief CANInterface class. Class is needed for setting up the connection
 * with the microcontrollers connected via CAN.
 * 
 */

class CANInterface{
public:
    CANInterface(const std::string& name);

    bool open();
    int getFd() const;
    bool readFrame(struct can_frame& frame);

    ~CANInterface();

private:
    std::string interfaceName;
    int sockfd;

};


#endif
