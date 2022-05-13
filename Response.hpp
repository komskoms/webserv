#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>
#include <map>
#include <vector>

#include <arpa/inet.h>  // inet_addr()
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socket(), accept(), listen(), bind(), connect(), recv()
#include <sys/time.h>   // struct timespec
#include <fcntl.h>  // fcntl()
#include <sys/event.h>  // struct kevent, kevent()
#include <unistd.h>

typedef std::map<std::string, std::string> StringMap;

// class to store data of HTTP request message.
// Example:
//  // example of data in request_string
//  // "GET /index.html HTTP/1.1
//  //  Host: localhost
//  //
//  //  ";
//  Request request(request_string);
//  if (request.getMethod() == Request::METHOD_GET)
//      server.runMessage(request, response);
class Request {
public:
    enum Method {
        METHOD_GET,
        METHOD_POST,
        METHOD_DELETE,
    };

    void append(const char* str) { this->message += str; };
    void parse();

    Method getMethod() const;
    char getMajorVersion() const;
    char getMinorVersion() const;

    bool isReadyToService();

    void describe(std::ostream& out) const; // this is for debug usage

private:
    std::string message;

    Method _method;
    std::string _request_target;
    char _major_version;
    char _minor_version;

    StringMap _headerFieldMap;

    std::string _body;
};

#endif  // REQUEST_HPP_
