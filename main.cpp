#include "ServerManager.hpp"

int main(int argc, char **argv) {
    ServerManager sm;

    if (argc != 2)
        std::cerr << "입력 인자 숫자 달라~\n";
    else
        sm.initParseConfig(argv[1]);
    // sm.initializeServers();
    int     portsOpen[2] = {2000, 2020};
    sm.initializeSocket(portsOpen, 2);
    sm.run();
    // sm.printAll();
    return 0;
}
