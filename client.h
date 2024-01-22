#pragma once

#include <iostream>
#include <thread>
#include <string>

class Client {
public:
    std::string name;
    std::string address;
    int socket;
    std::thread listener;

    explicit Client(int clientSocket, std::string clientAddress);
    ~Client();

    void startListening();
    void sendMessage(const std::string& msg); // Add this method

private:
    bool isListening;
};
