#ifndef DIAGNOSTICM_SERVER_H
#define DIAGNOSTICM_SERVER_H

#include "../Window/Window.h"
#include "IServer.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/io_service.hpp>

#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_ext.hpp"

#include <list>
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>

using namespace boost::asio;

class Server : public IServer {
private:
    io_service service;
    std::unique_ptr<ip::tcp::endpoint> ep;
    std::unique_ptr<ip::tcp::acceptor> acceptor;
    std::vector<ip::tcp::socket> sockets;

    std::vector<std::thread> threads;
    std::mutex listAccess;
    std::mutex socketAccess;
    std::mutex canSendAccess;
    std::mutex sendingAccess;

    std::shared_ptr<Window> windowPtr;
    std::list<cv::Point> mouseCoordinates;

    size_t pointsCount;

    bool canSend;
    bool sending;

    const std::string serverName {"SERVER"};

    void Draw();

    void Write(int x, int y);

    void WriteXML();

    void GetSockets();

    void Send();
public:
    Server(size_t pCount);

    void Start();

    void MouseEvent(int event, int x, int y, int flags);

    const std::string& GetName() const;

};


#endif //DIAGNOSTICM_SERVER_H
