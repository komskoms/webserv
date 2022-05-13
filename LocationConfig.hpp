#ifndef LOCATIONCONFIG_HPP_
#define LOCATIONCONFIG_HPP_

#include <string>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

class LocationConfig {
    public:
        LocationConfig(){}
        ~LocationConfig(){}
        void    setPath(std::string path);
        void    setHeader(std::map<std::string, std::vector<std::string> > headers);
        std::string getPath();
        std::map<std::string, std::vector<std::string> > getHeader();
        void    appendHeader(std::string fieldName, std::vector<std::string> fieldValue);
        bool    parsing(std::fstream &fs, std::stringstream &ss, std::string confLine);
    private:
        bool                _inBrace;
        std::string         _path;
        std::map<std::string, std::vector<std::string> > _headers;
};

#endif // LOCATIONCONFIG_HPP_