#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Config.h"
using std::cout;

enum Parse_Pos {
    NONE,
    IN_SERVER,
    IN_LOCATION
};

unsigned char parse_status; // for bitmask

int main(int argc, char **argv)
{
    std::ifstream config_file;
    std::string conf_line;
    std::string conf_origin = "";

    // ./webserve {configFile path}
    config_file.open(argv[1]);
    if (config_file.is_open()) { // check file open
        while (config_file >> conf_line) {
            conf_origin.append(conf_line + ' ');
        }
        config_file.close(); // must
        cout << "Done!!!" << '\n';
    }
    else
        cout << "Not open file" << '\n';
  
    cout << conf_origin;
    
    return 0;
}