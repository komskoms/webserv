#include "Socket.hpp"

Socket::Socket(int port)
: _client(false)
, _port(port) {
    setNewSocket();
    bindThisSocket();
    listenThisSocket();
}

Socket::Socket(int ident, std::string addr, int port)
: _client(true)
, _ident(ident)
, _addr(addr)
, _port(port) {
}

Socket*     Socket::acceptClient() {
    sockaddr_in     remoteaddr;
    socklen_t       remoteaddrSize = sizeof(remoteaddr);
    struct kevent   ev;
    int clientfd = accept(_ident, reinterpret_cast<sockaddr*>(&remoteaddr), &remoteaddrSize);
    std::string     addr;

    if (clientfd < 0)
    {
        throw std::runtime_error("accept() Failed");
        return NULL;
    }
    addr = inet_ntoa(remoteaddr.sin_addr);
    Log::Verbose("Connected from [%s:%d]", addr.c_str(), remoteaddr.sin_port);
    if (fcntl(clientfd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl Failed");
    return new Socket(clientfd, addr, remoteaddr.sin_port);
    // EV_SET(&ev, clientfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    // if (kevent(_kqueue, &ev, 1, 0, 0, 0) < 0)
    //  throw std::runtime_error("kevent Failed.");
}

void    Socket::receive() {
    unsigned char buffer[TCP_MTU];

    recv(_ident, buffer, TCP_MTU, 0);
    Log::Verbose("receive works");
}

void    Socket::transmit() {
    // TODO implement real behavior
    char buf[10] = "hi";
    send(this->_ident, buf, 2, 0);
//     sock.response.mesgae
//     numnbeOfBytes = ::send(fd, event.udata.message + pos, 남은 크기, 0);
//     event.udata.pos += numberOfBytes
}

void    Socket::addKevent(int kqueue, int filter, void* udata) {
    struct kevent   ev;

    EV_SET(&ev, _ident, filter, EV_ADD | EV_ENABLE, 0, 0, udata);
    if (kevent(kqueue, &ev, 1, 0, 0, 0) < 0)
        throw std::runtime_error("kevent Failed.");
}

bool            Socket::isclient() {
    return _client;
}

int             Socket::getIdent() {
    return _ident;
}

std::string     Socket::getAddr() {
    return _addr;
}

int             Socket::getPort() {
    return _port;
}

std::string     Socket::getHTTPMessage() {
    return _HTTPMessage;
}


void    Socket::setNewSocket() {
    int     newSocket = socket(PF_INET, SOCK_STREAM, 0);
    int     enable = 1;

    if (0 > newSocket) {
        throw;
    }
    Log::Verbose("New Server Socket ( %d )", newSocket);
    if (0 > setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        throw;
    }
    Log::Verbose("Socket ( %d ) has been setted to Reusable.", newSocket);
    _ident = newSocket;
}

static void setSocketAddr(int port, sockaddr_in& addr_in) {
    std::memset(&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = PF_INET;
    addr_in.sin_port = htons(port);
    // addr_in.sin_addr.s_addr = INADDR_ANY;
    Log::Verbose("Socketadd struct has been setted");
}

void    Socket::bindThisSocket() {
    sockaddr*   addr;
    sockaddr_in addr_in;
    setSocketAddr(_port, addr_in);
    addr = reinterpret_cast<sockaddr*>(&addr_in);
    if (0 > bind(_ident, addr, sizeof(*addr))) {
        throw;
    }
    Log::Verbose("Socket ( %d ) bind succeed.", socket);
}

void    Socket::listenThisSocket() {
    if (0 > listen(_ident, 10)) {
        throw;
    }
    Log::Verbose("Listening from Socket ( %d ), Port ( %d ).", _ident);
}
