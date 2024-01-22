#include "client.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <memory>

Client::Client(int clientSocket, std::string clientAddress)
    : socket(clientSocket), address(std::move(clientAddress)), isListening(true) {}

Client::~Client() {
    close(socket);

    // Check if the listener thread is joinable before joining
    if (listener.joinable()) {
        listener.join();
    }
}

void Client::startListening(MessageReceivedCallback onMessageReceivedCallback) {
    this->onMessageReceivedCallback = onMessageReceivedCallback;
    auto sharedThis = shared_from_this();  // Capture shared pointer

    listener = std::thread([this, sharedThis, onMessageReceivedCallback = std::move(onMessageReceivedCallback)]() mutable {
        char buffer[4096];
        while (isListening) {
            ssize_t bytesRead = recv(socket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                // Handle disconnection or error
                isListening = false;  // Set flag to stop the listener
                break;
            }

            buffer[bytesRead] = '\0';
            std::cout << "[" << address << "]: " << buffer << std::endl;

            if (onMessageReceivedCallback) {
                std::string receivedMsg(buffer, bytesRead);
                onMessageReceivedCallback(receivedMsg, sharedThis);
            }
        }
    });
}


void Client::sendMessage(const std::string& msg) {
    // Send the message to the client
    send(socket, msg.c_str(), msg.size(), 0);
}

void Client::setOnMessageReceivedCallback(MessageReceivedCallback callback) {
    onMessageReceivedCallback = callback;
}
