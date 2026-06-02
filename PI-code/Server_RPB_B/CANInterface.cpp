#include "CANInterface.h"
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>

// CANInterface constructor, needs the name of the interface (can0)
CANInterface::CANInterface(const std::string& name) : interfaceName(name), sockfd(-1)
{}

// Opening the CAN interface
bool CANInterface::open(){
    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd < 0) return false;

    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ - 1);

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0){
        close(sockfd);
        return false;
    }

    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        close(sockfd);
        return false;
    }

    return true;
}

int CANInterface::getFd() const{
    return sockfd;
}

bool CANInterface::readFrame(struct can_frame& frame){
    return read(sockfd, &frame, sizeof(frame)) > 0;
}

void CANInterface::sendCAN(uint32_t id, std::initializer_list<uint8_t> data){
    struct can_frame frame;

    if (data.size() > 8)
    return;

    frame.can_id = id;
    frame.can_dlc = data.size();

    int i = 0;
    for (uint8_t byte : data){
        frame.data[i++] = byte;
    }

    write(sockfd, &frame, sizeof(frame));
}

CANInterface::~CANInterface() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}
