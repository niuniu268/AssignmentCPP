#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <memory>

class Client;  // Forward declaration

class Server {
public:
    Server(const std::string& serverIp, int serverPort);
    ~Server();

    [[noreturn]] void start();

    void broadcast(const std::string& msg, std::shared_ptr<Client> client);

private:
    std::string ip;
    int port;
    int serverSocket;
    std::map<std::string, std::shared_ptr<Client>> onlineMap;
    std::mutex mapLock;

    void listenBroadcast();
};
