#include "LocationConfig.hpp"

void    LocationConfig::setPath(std::string path) {
    // if path is not null => error발생
    // if path not directory or file => error?
    this->_path = path;
}
void    LocationConfig::setHeader(std::map<std::string, std::vector<std::string> > headers) {
    this->_headers = headers;
}
std::string LocationConfig::getPath() {
    return this->_path;
}
std::map<std::string, std::vector<std::string> > LocationConfig::getHeader() {
    return this->_headers;
}
void    LocationConfig::appendHeader(std::string fieldName, std::vector<std::string> fieldValue){
    _headers.insert(make_pair(fieldName, fieldValue));
}
bool    LocationConfig::parsing(std::fstream &fs, std::stringstream &ss, std::string confLine)
{
    std::string token;
    std::string fn;
    std::vector<std::string> fv;

    ss >> token;
    if (token.find_first_of("/") != 0)
        return false;
    this->setPath(token);
    ss >> token;
    if (token != "{")
        return false;
    this->_inBrace = true; // (POSSIBLE) 삭제 가능
    while (true)
    {
        if (!ss) {
            if (!getline(fs, confLine)) // 스트림이 비어있고 가져올 줄이 없는 경우
                break ;
            ss.clear();
            ss << confLine;
        }
        if (!(ss >> token)) // \t도 인식하는 것 같음, lldb에서는 확인 안되지만
            continue;
        if (token == "}") {
            this->_inBrace = false;
            break;
        }
        else if (fn.empty()) // 필드 네임이 비어있는 경우
            fn = token;
        else if (token.substr(token.length() - 1) == ";") { // 마지막 인자의 ; 삭제후 저장
            fv.push_back(token.substr(0, token.length() - 1));
            this->_headers.insert(std::pair<std::string, std::vector<std::string> >(fn, fv));
            fn.clear(); // 필드 네임 / 밸류 초기화
            fv.clear(); //
        }
        else 
            fv.push_back(token);
    }
    return true;
}