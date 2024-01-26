#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <memory>
#include "manage.h"

class Client;

class Server {
public:
    Server(const std::string& serverIp, int serverPort);
    ~Server();

    [[noreturn]] void start();

    void sendMessageToAllClients(const std::string& msg);

private:
    std::string ip;
    int port;
    int serverSocket;
    std::map<std::string, std::shared_ptr<Client>> onlineMap;
    std::mutex mapLock;
    std::vector<std::shared_ptr<Client>> connectedClients;
    Manage crudManager;

    void listenBroadcast();
    void processMessage(const std::string& msg, std::shared_ptr<Client> sender);
};
