#include "Client.h"


int main() {
    std::string address;
    size_t port = 0;

    std::cin >> address >> port;

    Client *client = nullptr;

    try {
        client = new Client(address,port);
        client->Start();
    } catch (...) {
        std::cerr << "Can't start, check ip and port.\n";
        exit(155);
    }

    cv::waitKey(0);
    client->Disconnect();

    delete client;

    return 0;
}