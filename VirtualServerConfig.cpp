#include "VirtualServerConfig.hpp"

//  Constructor of VirtualServerConfig
//  - Parameters(None)
VirtualServerConfig::VirtualServerConfig()
: _inBrace(false){
}

//  Destructor of VirtualServerConfig
//  - Parameters(None)
VirtualServerConfig::~VirtualServerConfig() {
    for (std::set<LocationConfig *>::iterator itr = _locations.begin(); itr != _locations.end(); ++itr)
        delete *itr;
}

//  Append single config block to Config container
//  - Parameters
//      - directive: 
//      - value: 
//  - return(None)
void    VirtualServerConfig::appendConfig(std::string directive, std::vector<std::string> value) {
    std::map<std::string, std::vector<std::string> >::iterator itr = this->_configs.find(directive);
    if (itr->first == "error_page" && itr != this->_configs.end()) {
        for (std::vector<std::string>::iterator itr2 = value.begin();
            itr2 != value.end();
            itr2++)
        itr->second.push_back(*itr2);
    }
    else
        _configs.insert(make_pair(directive, value));
}

//  Parse and tokenize raw string to server block
//  - Parameters
//      fs: file stream with configuration file information 
//      ss: string steam for generating tokens in semantic unit
//      confLine: get one line from file stream(fs)
//  - return
//      If parsing success, it's true. if not, false.
bool    VirtualServerConfig::parsing(std::fstream &fs, std::stringstream &ss, std::string confLine)
{
    std::string token;
    std::string directive = "";
    std::vector<std::string> values;
    LocationConfig *lc;

    ss >> token;
    if (token != "{")
        return false;
    this->_inBrace = true;
    while (true) {
        if (ss.eof()) {
            if (!getline(fs, confLine))
                break ;
            ss.clear();
            ss << confLine;
        }
        if (!(ss >> token))
            continue;
        if (token == "location") {
            lc = new LocationConfig();
            if (!lc->parsing(fs, ss, confLine))
                return false;
            ss >> token;
            this->_locations.insert(lc);
        }
        else if (token == "}") {
            this->_inBrace = false;
            break;
        }
        else if (directive.empty())
            directive = token;
        else if (token.substr(token.length() - 1) == ";") {
            values.push_back(token.substr(0, token.length() - 1));
            this->appendConfig(directive, values);
            directive.clear();
            values.clear();
        }
        else
            values.push_back(token);
    }
    return true;
}
