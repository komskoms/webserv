#include <string>
#include <iostream>
#include <fstream>  
#include <sstream>
#include <set>
#include <map>
#include <vector>

class LocationConfig {
public:
    LocationConfig(){}
    ~LocationConfig(){}

private:
    std::string         _path;
    std::map<std::string, std::vector<std::string> > _headers;
};

class ServerConfig {
public:
    ServerConfig(){}
    ~ServerConfig(){}

private:
    std::set<LocationConfig>            _locations;  // 로케이션 블럭마다 다르게 저장
    std::map<std::string, std::string>  _configs;    // 순수 서버 설정값들, vector로 값 저장할 거 고려
};

class ServerManager {
public:
    ServerManager(){}
    ~ServerManager(){}
    void    initParseConfig(std::string filePath);
    //void    errorParsing(std::string cause) {}

private:
    std::set<ServerConfig>              _defaultConfigs; // 서버마다 속성값 다르기에 구분
};

void ServerManager::initParseConfig(std::string filePath) {
    std::fstream        fs;
    std::stringstream   ss;
    std::string         confLine;

    fs.open(filePath);
    if (fs.is_open()) {
        
    }
    else {
        std::cerr << "not open file\n";
        // error handle
    }
    
}

int main(int argc, char **argv) {
    ServerManager sm;
    sm.initParseConfig(argv[1]);
    return 0;
}