#include "Client.h"


Client::Client(const std::string &ip, size_t port) : areConnected(false) {
    windowPtr = std::make_shared<Window>(GetName());
    windowPtr->Show();

    ep = std::make_unique<ip::tcp::endpoint>(ip::address::from_string(ip), port);
    socket = std::make_unique<ip::tcp::socket>(service);
}

void Client::Connect() {
    socket->connect(*ep);
    areConnected = true;
}

void Client::Start() {
    Connect();
    readThread = std::thread([this](){ this->Read(); });
}

void Client::Read() {
    while (areConnected) {
        //socket->read_some(buffer(intArr));
        streambuf buf;
        read_until(*socket,buf, '\n');

        std::istream stream(&buf);

        char character = 0;
        std::string str;

        while (!stream.eof()) {
            stream.read(reinterpret_cast<char*>(&character), sizeof(char));
            if (!isdigit(character) && !str.empty()) {
                intArr.push_back(std::stoi(str));
                str.clear();
            }
            else if (isdigit(character))
                str.push_back(character);
        }

        buf.consume(intArr.size() + sizeof(char) * intArr.size());

        if (!intArr.empty()) {
            std::lock_guard<std::mutex> wa(windowAccess);
            for (int i = 0; i < intArr.size(); i += 2) {
                int x = windowPtr->GetSize().first - intArr[i];
                int y = windowPtr->GetSize().second - intArr[i + 1];

                cv::circle(windowPtr->GetRef(),
                           cv::Point(x,y),
                           1,
                           cv::Scalar(0, 0, 255),
                           cv::FILLED,
                           cv::LINE_8);
                windowPtr->Show();
            }
            intArr.clear();
        }
    }
}

void Client::WriteXML() {
    std::ofstream xml("client_output.xml");

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

const std::string &Client::GetName() {
    return name;
}

void Client::Disconnect() {
    areConnected = false;
    readThread.join();
}
