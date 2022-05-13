#include "Server.hpp"
#include "Socket.hpp"

void Server::process(Socket& socket, int kqueueFD) const {
//     socket.parseRequest();

//     Response response;
// 
//     response.setMajorVersion('1');
//     response.setMinorVersion('1');
//     response.setStatusCode("200");
//     response.setReasonPhrase("OK");
// 
//     response.clearHeaderFieldMap();
//     response.insertHeaderFieldMap("Server", "custom server");
//     response.insertHeaderFieldMap("Date", "Mon, 25 Apr 2022 05:38:34 GMT");
//     response.insertHeaderFieldMap("Content-Type", "image/png");
//     response.insertHeaderFieldMap("Last-Modified", "Tue, 04 Dec 2018 14:52:24 GMT");
// 
//     std::string str = std::string(0x1 << 10, 'a');
//     response.setBody(str);
// 
//     socket._response.message = convertToString();

    socket.addKevent(kqueueFD, EVFILT_WRITE, NULL);
}
