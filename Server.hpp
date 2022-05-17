#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <vector>
#include <map>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "Socket.hpp"
#include "Request.hpp"

class Server {
public:
        Server(const char* ipAddress, in_port_t portNumber, const std::string& serverName)
        : portNumber(portNumber), serverName(serverName) { inet_pton(AF_INET, ipAddress, &this->sinAddress); };

    in_port_t getPortNumber() const { return this->portNumber; };
    void setSock(Socket* sock) { this->sock = sock; };

    void process(Socket& socket, int kqueueFD) const;

private:
    struct in_addr sinAddress;
    in_port_t portNumber;
    std::string serverName;

    // 다른 많은 설정값

    Socket* sock;
};

#endif
