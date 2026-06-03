    #ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <string>
#include <linux/can.h>
#include <initializer_list>
#include <cstdint>

class CANInterface{
public:
    CANInterface(const std::string& name);

    bool open();
    int getFd() const;
    bool readFrame(struct can_frame& frame);
    void sendCAN(uint32_t id, std::initializer_list<uint8_t> data);

    ~CANInterface();

private:
    std::string interfaceName;
    int sockfd;

};


#endif
