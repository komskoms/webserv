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

void Connection::transmit() {
    // TODO implement real behavior
    char buf[10] = "hi";
    send(this->_ident, buf, 2, 0);
//     sock.response.mesgae
//     numnbeOfBytes = ::send(fd, event.udata.message + pos, 남은 크기, 0);
//     event.udata.pos += numberOfBytes
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
