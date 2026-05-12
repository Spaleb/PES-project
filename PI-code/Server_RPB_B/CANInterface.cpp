#include "CANInterface.h"
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

/**
 * @brief Construct a new CANInterface::CANInterface object
 * 
 * @param name -> name of the can interface (can0)
 */
CANInterface::CANInterface(const std::string& name) : interfaceName(name), sockfd(-1)
{}

/**
 * @brief Opens the CAN socket connection
 * 
 * @return true if succeeded
 * @return false if failed
 */
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

/**
 * @brief Return the sockfd value 
 * 
 * @return int 
 */
int CANInterface::getFd() const{
    return sockfd;
}

/**
 * @brief Simple function for reading the received can frame
 * 
 * @param frame 
 * @return true 
 * @return false 
 */
bool CANInterface::readFrame(struct can_frame& frame){
    return read(sockfd, &frame, sizeof(frame)) > 0;
}

/**
 * @brief Destroy the CANInterface::CANInterface object if the connection is closed
 * 
 */
CANInterface::~CANInterface() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}
