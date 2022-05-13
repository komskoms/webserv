#include "ServerConfig.hpp"

void    ServerConfig::appendConfig(std::string directive, std::vector<std::string> value) {
    _configs.insert(make_pair(directive, value));
}
bool    ServerConfig::parsing(std::fstream &fs, std::stringstream &ss, std::string confLine)
{
    std::string token;
    std::string directive = "";
    std::vector<std::string> values;
    LocationConfig *lc;

    ss >> token;
    if (token != "{")
        return false;
    this->_inBrace = true;
    // token.clear();
    ss >> token;
    while (true) {
        if (!ss) {
            if (!getline(fs, confLine)) // 스트림이 비어있고 가져올 줄이 없는 경우
                break ;
            ss.clear();
            ss << confLine;
        }
        if (!(ss >> token)) // \t도 인식하는 것 같음, lldb에서는 확인 안되지만
            continue;
        if (token == "location") {
            lc = new LocationConfig();
            if (!lc->parsing(fs, ss, confLine))
                return false;
            ss >> token; // '{' (TODO) error handle
            this->_locations.insert(lc); // (TODO) function 만들 것
        }
        else if (token == "}") {// close brace
            this->_inBrace = false;
            break;
        }
        else if (directive.empty())
            directive = token;
        else if (token.substr(token.length() - 1) == ";") {
            values.push_back(token.substr(0, token.length() - 1)); // vector push_back
            this->appendConfig(directive, values);
            directive.clear();
            values.clear();
        }
        else
            values.push_back(token); // vector push_back
    }
    return true;
}
bool    ServerConfig::getInBrace(){
    return this->_inBrace;
}
std::set<LocationConfig *> ServerConfig::getLocations(){
    return this->_locations;
}
std::map<std::string, std::vector<std::string> > ServerConfig::getConfigs(){
    return this->_configs;
}