#ifndef SERVERCONFIG_HPP_
#define SERVERCONFIG_HPP_

#include <set>
#include <string>
#include <map>
#include <fstream>
#include "LocationConfig.hpp"

class ServerConfig {
    public:
        ServerConfig(){_inBrace = false;}
        ~ServerConfig(){
            for (std::set<LocationConfig *>::iterator itr = _locations.begin(); itr != _locations.end(); ++itr) {
                delete *itr;
            }
        }
        bool    parsing(std::fstream &fs, std::stringstream &ss, std::string confLine);
        void    appendConfig(std::string directive,  std::vector<std::string> value);   // 새로운 속성 추가
        bool    getInBrace();
        std::set<LocationConfig *> getLocations();
        std::map<std::string, std::vector<std::string> > getConfigs();

    private:
        bool                               _inBrace;
        std::set<LocationConfig *>          _locations;  // 로케이션 블럭마다 다르게 저장
        std::map<std::string, std::vector<std::string> >  _configs;    // 순수 서버 설정값들, vector로 값 저장할 거 고려
};
#endif //SERVERCONFIG_HPP_