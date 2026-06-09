#include "CANInterface.h"
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>

/**
 * @brief Construeert een nieuw CANInterface::CANInterface object.
 * 
 * @param name is de naam van het CAN-interface (can0)
 */
CANInterface::CANInterface(const std::string& name) : interfaceName(name), sockfd(-1)
{}

// Openen van het CAN-interface.
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
 * @brief returnd de sockfd van de CAN socket
 * 
 * @return int return int sockfd, is een referentie naar de file descriptor code (in dit geval CAN).
 */
int CANInterface::getFd() const{
    return sockfd;
}

/**
 * @brief Leest een CAN-frame uit en returned daarna of het succesvol uitgelezen is. 
 * 
 * @param frame Referenctie naar CAN-struct (layout van een CAN-frame).
 * @return true Als er een frame is gelezen.
 * @return false Als het niet gelukt is of er niks gelezen is.
 */
bool CANInterface::readFrame(struct can_frame& frame){
    return read(sockfd, &frame, sizeof(frame)) > 0;
}

/**
 * @brief Verantwoordelijk voor het versturen van een CAN-bericht naar de STM32 microcontrollers.
 *
 * @param id Het ID waarmee het CAN-bericht wordt worden verstuurd. Op basis van dit ID
 Kan een STM32 Microcontroller juist interpreteren wat er moet gebeuren.
 * @param data De data die verstuurd moet worden in hex, in de vorm van een list. 
 */
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

/**
 * @brief Verwijderen van het CANInterface::CANInterface object.
 * 
 */
CANInterface::~CANInterface() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}
