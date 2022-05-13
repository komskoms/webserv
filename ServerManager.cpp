#include "ServerManager.hpp"

void ServerManager::initParseConfig(std::string filePath) {
    std::fstream        fs;
    std::stringstream   ss;
    std::string         confLine = "";
    std::string         token;
    ServerConfig        *sc;

    fs.open(filePath);
    if (fs.is_open()) {
        while (getline(fs, confLine)) {
            ss << confLine;
            if (!(ss >> token))
            {
                ss.clear();
                continue;
            }
            else if (token == "server") {
                sc = new ServerConfig();
                if(!sc->parsing(fs, ss, confLine)) // fstream, stringstream를 전달해주는 방식으로 진행
                    std::cerr << "not parsing config\n"; // 각 요소별 동적할당 해제시켜주는게 중요
                this->_defaultConfigs.insert(sc);
            } else
                std::cerr << "not match (token != server)\n"; // (TODO) 오류터졌을 때 동적할당 해제 해줘야함
            ss.clear();
        }
    }
    else
        std::cerr << "not open file\n";
    
}
void    ServerManager::printAll()
{
    for (std::set<ServerConfig *>::iterator itr = this->_defaultConfigs.begin(); itr != this->_defaultConfigs.end(); itr++)
    {
        std::map<std::string, std::vector<std::string> > sc = (*itr)->getConfigs();
        std::cout << "\n=========== server origin config ===========\n";
        for (auto i : sc) {
            std::cout << "key : "<< i.first << " values : ";
            for (auto value : i.second) {
                    std::cout << value << ' ';
            }
            std::cout << "\n";
        }
        
        std::set<LocationConfig *> lc = (*itr)->getLocations();
        for (auto l : lc) {
            std::string p = l->getPath();
            std::map<std::string, std::vector<std::string> > msv = l->getHeader();
            std::cout << "\n=========== location        config ===========\n";
            std::cout << "path : " << p << '\n';
            for (auto j : msv) {
                std::cout << "key : " << j.first << " values : ";
                for (auto value : j.second) {
                    std::cout << value << ' ';
                }
                std::cout << "\n";
            }
        }
    }
}
