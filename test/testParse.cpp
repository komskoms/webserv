#include <string>
#include <iostream>
#include <fstream>  
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <utility>

 // using headerField = pair<std::string, std::vector<std::string> >;

class LocationConfig {
    public:
        LocationConfig(){}
        ~LocationConfig(){}
        void    setPath(std::string path);
        void    setHeader(std::string, std::vector<std::string> headers);
        void    initPath(std::string path); // 로케이션 1블럭 = 1 path
        void    appendHeader(std::string fieldName, std::vector<std::string> fieldValue);   // 새로운 헤더필드 추가
        // bool    parsing();
    private:
        std::string         _path;
        std::map<std::string, std::vector<std::string> > _headers;
};

void    LocationConfig::initPath(std::string path){
    // if path is not null => error발생
    // if path not directory or file => error?
    this->_path = path;
}

void    LocationConfig::appendHeader(std::string fieldName, std::vector<std::string> fieldValue){
    // std::string tFieldName = new std::string(fieldName);
    // std::vector<std::string> *tFieldValue = new std::vector<std::string>(fieldValue);
    // std::cout << *tFieldName << '\n';
    // delete tFieldName;
    _headers.insert(make_pair(fieldName, fieldValue));
}

class ServerConfig {
    public:
        ServerConfig(){}
        ~ServerConfig(){}
        void    makeLocationBlock();
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
    std::string         confLine = "";
    std::string         token;
    std::vector<std::string> tmp;
    short               nowPos = 0;
    tmp.push_back("hello");
    tmp.push_back("world");
    tmp.push_back("korea");
    
    fs.open(filePath);
    if (fs.is_open()) {
        LocationConfig * lc_test;
        // in location
        std::map<std::string, std::vector<std::string> > testHeaders;
        std::string fn = "";
        std::vector<std::string> fv;
        lc_test = new LocationConfig();
        // in location
        while (getline(fs, confLine)) {
            ss << confLine;
            while (ss >> token) {
                // in location test code
                if (token == "location") { //
                    ss >> token; // /{path}
                    lc_test->initPath(token);
                    nowPos |= (1 << 1);
                    ss >> token; // '{' (TODO) error handle
                } else if (nowPos & 1 << 1){ // in location block?
                    if (token == "}") { // close brace
                        nowPos &= ~(1 << 1); // 해제
                    }
                    else if (fn.empty())
                        fn = token;
                    else if (token.substr(token.length() - 1) == ";") {
                        fv.push_back(token.substr(0, token.length() - 1));
                        testHeaders.insert(std::pair<std::string, std::vector<std::string> >(fn, fv));
                        fn.clear(); // 동적할당으로 안할시 이렇게 초기화 해줘야함 
                        fv.clear(); // 
                    }
                    else 
                        fv.push_back(token);
                }
                // end location test code
            }
            ss.clear();
        }
        std::cout << "hello world";
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