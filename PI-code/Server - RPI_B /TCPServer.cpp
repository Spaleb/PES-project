#include "TCPServer.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * @brief Construct a new TCPServer::TCPServer object
 * 
 */
TCPServer::TCPServer(){}

/**
 * @brief Opening the TCP socket and listening for available clients
 * 
 * @param port (8080)
 * @return true 
 * @return false 
 */
bool TCPServer::start(int port){
    sockaddr_in address{};
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return false;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0)
        return false;

    if (listen(server_fd, 3) < 0)
        return false;

    return true;
}


/**
 * @brief Accepting the request of pending clients.
 * Clients can connect at all time, also after being disconnected.
 * 
 * @return int 
 */
int TCPServer::acceptClient(){
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);

    client_fd = accept(server_fd, (sockaddr*)&addr, &len);
    return client_fd;
}
/**
 * @brief Returns the client_fd
 * 
 * @return int client_fd
 */
int TCPServer::getCLientFd() const{
    return client_fd;
}

/**
 * @brief Return the server_fd
 * 
 * @return int server_fd
 */
int TCPServer::getServerFd() const{
    return server_fd;
}

/**
 * @brief Reads the messages that have been send over TCP
 * 
 * @param buffer 
 * @param size 
 * @return int 
 */
int TCPServer::readClient(char* buffer, int size){
    return read(client_fd, buffer, size);
}

int TCPServer::sendClient(const std::string& msg) {
    return send(client_fd, msg.c_str(), msg.size(), 0);
}

/**
 * @brief Destroy the TCPServer::TCPServer object
 * 
 */
TCPServer::~TCPServer(){
    if (client_fd >= 0) {
        close(client_fd);
    }

    if (server_fd >= 0) {
        close(server_fd);
    }
}
