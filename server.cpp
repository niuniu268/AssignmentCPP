#include "server.h"
#include "client.h"
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

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

        connectedClients.push_back(newClient);  // Store the connected client

        sendMessageToAllClients("online\n");

        newClient->startListening([this](const std::string& msg, std::shared_ptr<Client> sender) {
            processMessage(msg, sender);
        });

        std::string welcomeMsg = "Welcome to the chat!\n";
        newClient->sendMessage(welcomeMsg);


        // if (msg.substr(0, 3) == "add") {
        //     crudManager.Add_Acc(msg.substr(4)); // Extract the name after "add"
        //     sender->sendMessage("success\n");
        // }
        // // Add other CRUD command handling logic here...
        // else {
        //     // Process non-CRUD messages
        //     // ...
        // }
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

        // For this example, we'll simply read from standard input,
        // but you could replace this with any mechanism to get messages.
        std::getline(std::cin, msg);

        if (!msg.empty()) {
            sendMessageToAllClients(msg + "\n");  // Send the message to all clients
        }
    }
}

// void Server::processMessage(const std::string& msg, std::shared_ptr<Client> sender) {
//     // Process the incoming message, modify it, and send it back to the clients
//     std::string modifiedMsg = "[" + sender->address + "]: " + msg + " " + std::to_string(msg.size());
//     sendMessageToAllClients(modifiedMsg + "\n");
// }
void Server::processMessage(const std::string& msg, std::shared_ptr<Client> sender) {
    if (msg.substr(0, 3) == "add") {
        crudManager.Add_Acc(msg.substr(4)); // Extract the name after "add"
        sender->sendMessage("success\n");
    }
    // Add other CRUD command handling logic here...
    else {
        // Process non-CRUD messages
        // ...
    }
}
