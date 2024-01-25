#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <functional>

class Client : public std::enable_shared_from_this<Client> {
public:
    std::string name;
    std::string address;
    int socket;
    std::thread listener;

    using MessageReceivedCallback = std::function<void(const std::string&, std::shared_ptr<Client>)>;

    explicit Client(int clientSocket, std::string clientAddress);
    ~Client();

    void startListening(std::function<void(const std::string&, std::shared_ptr<Client>)> onMessageReceivedCallback);
    void sendMessage(const std::string& msg);
    void setOnMessageReceivedCallback(MessageReceivedCallback callback);

private:
    bool isListening;
    MessageReceivedCallback onMessageReceivedCallback;
};
