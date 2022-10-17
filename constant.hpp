#ifndef CONSTANT_HPP_
#define CONSTANT_HPP_

const int BUF_SIZE = 0x1 << 16;
const unsigned int MAX_WRITEBUFFER = 0x1 << 17;
const int LISTEN_BACKLOG = 40;
const int DEFAULT_CLIENT_MAX_BODY_SIZE = 100000;
const int TIMEOUT = 40000000;
const std::string DEFAULT_CONF_PATH = "./conf/sample_for_tester.conf";

#define EMPTY_CGI_RESPONSE "HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=utf-8\r\n\
Content-Length: 0\r\n\
\r\n"

#endif  // CONSTANT_HPP_