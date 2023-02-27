cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME DiagnosticM)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenCV REQUIRED)

set(RapidXML_FILES
        RapidXML/rapidxml.hpp
        RapidXML/rapidxml_ext.hpp
        RapidXML/rapidxml_iterators.hpp
        RapidXML/rapidxml_print.hpp
        RapidXML/rapidxml_utils.hpp
        RapidXML/RapidXMLSTD.cpp
        RapidXML/RapidXMLSTD.hpp
        )

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
    )

add_executable(SERVER_${PROJECT_NAME}
        Server/IServer.h
        Server/ServerStart.cpp
        Server/Server.cpp
        Server/Server.h
        Window/IWindow.h
        Window/Window.cpp
        Window/Window.h
        ${RapidXML_FILES}
        )

add_executable(CLIENT_${PROJECT_NAME}
        Client/IClient.h
        Client/ClientStart.cpp
        Client/Client.cpp
        Client/Client.h
        Window/IWindow.h
        Window/Window.cpp
        Window/Window.h
        ${RapidXML_FILES}
        )

target_link_libraries(SERVER_${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        )

target_link_libraries(CLIENT_${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        )
