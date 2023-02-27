#include "Server.h"

//#include <boost/asio/impl/src.hpp>

void MouseEvent(int event, int x, int y, int flags, void* userdata) {
    auto castedData = reinterpret_cast<Server*>(userdata);
    castedData->MouseEvent(event, x, y, flags);
}

int main() {
    std::ifstream xml("../count.xml");

    std::vector<char> buffer((std::istreambuf_iterator<char>(xml)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml.close();

    rapidxml::xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    rapidxml::xml_node<> * root_node = doc.first_node("N");

    std::string strN = root_node->value();
    int n = std::stoi(strN);

    Server object(n);

    cv::setMouseCallback(object.GetName(), MouseEvent, &object);
    object.Start();

    cv::waitKey(0);

    return 0;
}
