#include <cassert>
#include "Connection.hpp"

Connection::Connection(int port)
: _client(false)
, _port(port) {
    newSocket();
    bindSocket();
    listenSocket();
}

Connection::Connection(int ident, std::string addr, int port)
: _client(true)
, _ident(ident)
, _addr(addr)
, _port(port) {
}

Connection* Connection::acceptClient() {
    sockaddr_in     remoteaddr;
    socklen_t       remoteaddrSize = sizeof(remoteaddr);
    struct kevent   ev;
    int clientfd = accept(_ident, reinterpret_cast<sockaddr*>(&remoteaddr), &remoteaddrSize);
    std::string     addr;

    if (clientfd < 0) {
        throw std::runtime_error("accept() Failed");
        return NULL;
    }
    addr = inet_ntoa(remoteaddr.sin_addr);
    Log::Verbose("Connected from [%s:%d]", addr.c_str(), remoteaddr.sin_port);
    if (fcntl(clientfd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl Failed");
    return new Connection(clientfd, addr, remoteaddr.sin_port);
}

void Connection::receive() {
    unsigned char buffer[TCP_MTU];

    recv(_ident, buffer, TCP_MTU, 0);
    Log::Verbose("receive works");
}

//  Send response message to client.
void    Connection::transmit() {
    const int returnValue = this->_response.sendResponseMessage(this->_ident);
    switch (returnValue) {
        case -1:
            // TODO Implement behavior.
            break;
        case 0:
            // TODO Implement behavior.
            break;
        case 1:
            break;
        case 2:
            // TODO Implement behavior.
            break;
        default:
            assert(false);
            break;
    }
}

void Connection::addKevent(int kqueue, int filter, void* udata) {
    struct kevent   ev;

    EV_SET(&ev, _ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);
    if (kevent(kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("kevent Failed.");
}

void Connection::newSocket() {
    int     newConnection = socket(PF_INET, SOCK_STREAM, 0);
    int     enable = 1;

    if (0 > newConnection) {
        throw;
    }
    Log::Verbose("New Server Connection ( %d )", newConnection);
    if (0 > setsockopt(newConnection, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        throw;
    }
    Log::Verbose("Connection ( %d ) has been setted to Reusable.", newConnection);
    _ident = newConnection;
}

static void setAddrStruct(int port, sockaddr_in& addr_in) {
    std::memset(&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = PF_INET;
    addr_in.sin_port = htons(port);
    // addr_in.sin_addr.s_addr = INADDR_ANY;
    Log::Verbose("Connectionadd struct has been setted");
}

void Connection::bindSocket() {
    sockaddr*   addr;
    sockaddr_in addr_in;
    setAddrStruct(_port, addr_in);
    addr = reinterpret_cast<sockaddr*>(&addr_in);
    if (0 > bind(_ident, addr, sizeof(*addr))) {
        throw;
    }
    Log::Verbose("Connection ( %d ) bind succeed.", socket);
}

void Connection::listenSocket() {
    if (0 > listen(_ident, 10)) {
        throw;
    }
    Log::Verbose("Listening from Connection ( %d ), Port ( %d ).", _ident);
}

//  Set socket's response message.
//  If you want to append response message in Server object rather than in
//  Socket object, make a method named
//  Socket::appendResponseMessage(const char* message) which is calling
//  this->_response.appendMessage(message).
void Connection::setResponse() {
    //  TODO Implement real behavior.
    this->_response.appendMessage("HTTP/1.1 418 Custom Response\nServer: webserv\nDate: What time is it now?\nContent-Type: text/html\nContent-Length: 176\nConnection: close\n\n<html>\n<head><title>418 Custom Response</title></head>\n<body bgcolor=\"white\">\n<center><h1>418 Custom Response</h1></center>\n<hr><center>webserv</center>\n</body>\n</html>");
}
