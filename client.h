#pragma once

#include <iostream>
#include <thread>

class Client {
public:
    std::string name;
    std::string address;
    int socket;
    std::thread listener;

    explicit Client(int clientSocket, std::string clientAddress);
    ~Client();

    void startListening();

private:
    bool isListening;  // Add a flag to indicate whether the listener is active

    // Add any private member variables or methods as needed
};
