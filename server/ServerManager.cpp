#include "ServerManager.hpp"


ServerManager::ServerManager() :
_kqueue(-1),
_alive(true)
{
	Log::Verbose("A ServerManager has been generated.");
}

ServerManager::~ServerManager() {
	SocketMapIter	sockIter = _mSocket.begin();
	for (;sockIter != _mSocket.end() ; sockIter++) {
		delete sockIter->second;
	}
	Log::Verbose("All Sockets has been deleted.");
	// close(_kqueue);
}


void 	ServerManager::initParseConfig(std::string filePath) {
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

void 	ServerManager::initializeSocket(int ports[], int size) {
	_kqueue = kqueue();
	Log::Verbose("kqueue generated: ( %d )", _kqueue);
	for (int i = 0; i < size; i++) {
		Socket* newSocket = new Socket(ports[i]);
		_mSocket.insert(std::make_pair(newSocket->getIdent(), newSocket));
		try {
			newSocket->addKevent(_kqueue, EVFILT_READ);
		} catch(std::exception& exep) {
			Log::Verbose(exep.what());
		}
		Log::Verbose("Socket Generated: [%d]", ports[i]);
	}
}

void	ServerManager::clientAccept(Socket* socket) {
	Socket* newSocket = socket->acceptClient();
	_mSocket.insert(std::make_pair(newSocket->getIdent(), newSocket));
	try {
		newSocket->addKevent(_kqueue, EVFILT_READ);
	} catch(std::exception& excep) {
		Log::Verbose(excep.what());
	}
	Log::Verbose("Client Accepted: [%s]", newSocket->getAddr().c_str());
}

void	ServerManager::read(Socket* socket) {
	socket->receive();
}

void	ServerManager::write(Socket* socket) {
	socket->transmit("suppose to message"); /////////////////
}

void	ServerManager::run() {
    const int MaxEvents = 20;
    struct kevent events[MaxEvents];

    while (_alive == true)
    {
        int numbers = kevent(_kqueue, NULL, 0, events, MaxEvents, NULL);
        if (numbers < 0)
        {
            Log::Verbose("Server::run kevent error [%d]", errno);
            continue;
        }
        Log::Verbose("kevent found %d events.", numbers);
        for (int i = 0; i < numbers; i++)
        {
            // struct kevent& event = events[i];
            int filter = events[i].filter;
			Socket* eventSocket = _mSocket[events[i].ident];

            try
            {
                if (filter == EVFILT_READ && eventSocket->isclient() == false)
                    clientAccept(eventSocket);
                else if (filter == EVFILT_READ)
                    read(eventSocket);
                else if (filter == EVFILT_WRITE)
                    write(eventSocket);
            }
            catch (const std::exception& excep)
            {
                Log::Verbose("Server::run Error [%s]", excep.what());
            }
        }
    }
}
