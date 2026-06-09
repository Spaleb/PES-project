#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>

/**
 * @brief Header van TCPServer.cpp.
 * 
 */
class TCPServer{
public:
    TCPServer();

    bool start(int port);
    int acceptClient();
    int getClientFd() const;
    int getServerFd() const;
    int readClient(char* buffer, int size);
    int sendClient(const std::string& msg);

    ~TCPServer();

private:
    int server_fd = -1;
    int client_fd = -1;
};

#endif
