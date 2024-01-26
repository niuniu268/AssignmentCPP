#include "server.h"
#include "client.h"
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <vector>

Server::Server(const std::string& serverIp, int serverPort)
    : ip(serverIp), port(serverPort), serverSocket(-1) {}

Server::~Server() {
    close(serverSocket);
}

[[noreturn]] void Server::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::thread(&Server::listenBroadcast, this).detach();

    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::shared_ptr<Client> newClient = std::make_shared<Client>(clientSocket, inet_ntoa(clientAddress.sin_addr));

        mapLock.lock();
        onlineMap[newClient->address] = newClient;
        mapLock.unlock();

        connectedClients.push_back(newClient);

        sendMessageToAllClients("online\n");

        newClient->startListening([this](const std::string& msg, std::shared_ptr<Client> sender) {
            processMessage(msg, sender);
        });

        std::string welcomeMsg = "Please input update show delete or add\n";
        newClient->sendMessage(welcomeMsg);

    }
}

void Server::sendMessageToAllClients(const std::string& msg) {
    mapLock.lock();
    for (const auto& client : connectedClients) {
        client->sendMessage(msg);
    }
    mapLock.unlock();
}

void Server::listenBroadcast() {
    while (true) {
        std::string msg;

        std::getline(std::cin, msg);

        if (!msg.empty()) {
            sendMessageToAllClients(msg + "\n"); 
        }
    }
}

std::vector<std::string> split(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> choices;
    std::string choice;

    while (iss >> choice) {
        choices.push_back(choice);
    }

    return choices;
}

void Server::processMessage(const std::string& msg, std::shared_ptr<Client> sender) {
    std::vector<std::string> choices = split(msg);

    if (choices.empty()) {
        sender->sendMessage("wrong input\n");
        return;
    }

    const std::string& command = choices[0];

    if (command == "add" && choices.size() > 1) {
        std::string answer = crudManager.Add_Acc(choices[1]);
        sender->sendMessage(answer+"\n");
    } else if (command == "show") {
        crudManager.Show_Acc();
        for (int i = 0; i < crudManager.m_Num; i++)
        {
            sender->sendMessage(crudManager.m_Array[i]->m_Name + std::string("\n"));
        }
        
    } else if (command == "delete" && choices.size() > 1) {
        std::string answer = crudManager.Del_Acc(choices[1]);
        sender->sendMessage(answer+"\n");
    } else if (command == "update" && choices.size() > 2) {
        std::string answer = crudManager.Mod_Acc(choices[1], choices[2]);
        sender->sendMessage(answer+"\n");
    } else if (command == "search" && choices.size() > 1) {
        std::string answer = crudManager.Search_Acc(choices[1]);
        sender->sendMessage(answer + "\n");
    } else {
        sender->sendMessage("wrong input\n");
    }
}

