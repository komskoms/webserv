#ifndef SERVERMANAGER_HPP_
#define SERVERMANAGER_HPP_

#include <iostream>
#include "ServerConfig.hpp"

class ServerManager {
    public:
        ServerManager(){}
        ~ServerManager(){
            for (std::set<ServerConfig *>::iterator itr = _defaultConfigs.begin(); itr != _defaultConfigs.end(); ++itr) {
                delete *itr;
            }
        }
        void    initParseConfig(std::string filePath);
        void    printAll();
        // void    errorParsing(std::string cause) {}
    private:
        std::set<ServerConfig *>        _defaultConfigs;    // 서버마다 속성값 다르기에 구분
};

#endif //SERVERMANAGER_HPP_
