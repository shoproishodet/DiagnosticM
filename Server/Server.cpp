#include "Server.h"


Server::Server(size_t pCount) : pointsCount(pCount) {
    windowPtr = std::make_shared<Window>(GetName());
    windowPtr->Show();

    ep = std::make_unique<ip::tcp::endpoint>(ip::tcp::v4(), 8001);
    acceptor = std::make_unique<ip::tcp::acceptor>(service, *ep);

    canSend = false;
    sending = false;
}

void Server::MouseEvent(int event, int x, int y, int flags) {
    if (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON) {
        Write(x, y);
        Draw();

        bool flag = false;

        while (!flag) {
            std::lock_guard<std::mutex> lock(sendingAccess);
            if (!sending)
                flag = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
        }

        std::lock_guard<std::mutex> lock(canSendAccess);
        canSend = true;
    } else if (event == cv::EVENT_MOUSEMOVE) {
        Write(x, y);
    } else if (event == cv::EVENT_LBUTTONDOWN) {
        Draw();

        bool flag = false;

        while (!flag) {
            std::lock_guard<std::mutex> lock(sendingAccess);
            if (!sending)
                flag = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
        }

        std::lock_guard<std::mutex> lock(canSendAccess);
        canSend = true;
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        windowPtr->Clear();
    }
}

const std::string &Server::GetName() const {
    return serverName;
}

void Server::Draw() {
    for (auto &it: mouseCoordinates) {
        cv::circle(windowPtr->GetRef(),
                   it,
                   1,
                   cv::Scalar(0, 0, 255),
                   cv::FILLED,
                   cv::LINE_8);
        windowPtr->Show();
    }
}

void Server::Write(int x, int y) {
    //std::cout << x << "\t" << y << std::endl;

    if (!mouseCoordinates.empty() && mouseCoordinates.size() == pointsCount)
        mouseCoordinates.pop_front();

    mouseCoordinates.emplace_back(x, y);
}

void Server::GetSockets() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ip::tcp::socket sock(service);
        acceptor->accept(sock);

        if (sock.is_open()) {
            std::lock_guard<std::mutex> lockGuard(socketAccess);
            sockets.emplace_back(std::move(sock));
        }
    }
}

void Server::Send() {
    while (true) {
        bool flag = false;

        while (!flag) {
            {
                std::lock_guard<std::mutex> lock(canSendAccess);
                if (canSend)
                    flag = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
        }

        {
            std::lock_guard<std::mutex> lock(sendingAccess);
            sending = true;
        }

        std::stringstream buff;

        {
            std::lock_guard<std::mutex> lock(listAccess);
            for (auto &p : mouseCoordinates) {
                buff << p.x << ' ' << p.y << ' ';
            }
            buff << '\n';
        }

        {
            std::lock_guard<std::mutex> lock(socketAccess);
            for (auto &s: sockets)
                s.write_some(buffer(buff.str()));
        }

        WriteXML();

        {
            std::lock_guard<std::mutex> lockCanSend(canSendAccess);
            canSend = false;
        }

        {
            std::lock_guard<std::mutex> lockSending(sendingAccess);
            sending = false;
        }
    }
}

void Server::Start() {
    std::cout << "Wait clients on: " << ep->address() << "\t" << ep->port() << std::endl;

    threads.emplace_back([this]{ this->GetSockets(); });
    threads.emplace_back([this]{ this->Send(); });
}

void Server::WriteXML() {
    std::ofstream xml("server_output.xml");

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_type::node_declaration);

    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));

    rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_type::node_element, "Coordinates");

    for (auto &p : mouseCoordinates) {
        rapidxml::xml_node<> *pair = doc.allocate_node(rapidxml::node_type::node_element, "Pair");

        rapidxml::xml_node<> *x = doc.allocate_node(rapidxml::node_type::node_element, "X");
        rapidxml::xml_node<> *y = doc.allocate_node(rapidxml::node_type::node_element, "Y");

        x->value(std::to_string(p.x).c_str());
        y->value(std::to_string(p.y).c_str());

        pair->append_node(x);
        pair->append_node(y);

        root->append_node(pair);
    }

    doc.append_node(root);

    std::string xmlStr;
    rapidxml::print(std::back_inserter(xmlStr), doc);

    xml << xmlStr;

    xml.close();
}

