#include <iostream>
#include <fstream>
#include <string>

using std::cout;

int main(int argc, char **argv)
{
    std::ifstream config;
    std::string line;

    // ./webserve {config path}
    config.open(argv[1]);
    if (config.is_open()) { // check file open
        while (getline(config, line))
            cout << line << '\n';
        config.close(); // must
        cout << "Done!!!" << '\n';
    }
    else
        cout << "Not open file" << '\n';
    return 0;
}