#ifndef DIAGNOSTICM_CLIENT_H
#define DIAGNOSTICM_CLIENT_H

#include "../Window/Window.h"
#include "IClient.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_ext.hpp"

#include <memory>
#include <mutex>
#include <thread>
#include <fstream>

using namespace boost::asio;

class Client : public IClient {
private:
    io_service service;
    bool areConnected;

    std::unique_ptr<ip::tcp::socket> socket;
    std::unique_ptr<ip::tcp::endpoint> ep;

    std::vector<int> intArr;

    std::thread readThread;
    std::mutex windowAccess;

    std::shared_ptr<Window> windowPtr;
    std::vector<cv::Point> mouseCoordinates;

    std::string name {"CLIENT"};

    void WriteXML();

    void Read();

    void Connect();
public:
    Client(const std::string &ip, size_t port);

    void Start();

    void Disconnect();

    const std::string& GetName();
};


#endif //DIAGNOSTICM_CLIENT_H
