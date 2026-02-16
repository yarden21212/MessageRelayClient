/*
    Client application
*/
#include "NetworkBelonging.h"
#include "ClientConnection.h"
#include "ClientMenu.h";

using boost::asio::ip::tcp;
ClientMenu menu;

void clear(char* buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i] = '\0';
    }
}
int TCPConnector(std::string_view ip, std::string_view port) {
    const int max_length = 1024;

    try {

        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        //boost::asio::connect(socket, resolver.resolve(argv[1], argv[2]));
        boost::asio::connect(socket, resolver.resolve(ip, port));

        for (;;) {

            menu.DisplayOptions();
            char request[max_length];
            clear(request, max_length);
            std::cout << "Enter a message: ";
            std::cin.getline(request, max_length);

            ClientMenu::Menu decision = menu.convertToMenuOption(request);
            std::string toSend = menu.PickMenuOption(decision);
            std::cout << "Option chosen: " << toSend << std::endl;

            // If exit option was chosen, terminates the program.
            if (toSend.compare("exit") == 0) {
                std::cout << "[Debug]: " << toSend << std::endl;
                std::exit(0);
            }
            boost::asio::write(socket, boost::asio::buffer(request));

            char respond[max_length];
            boost::asio::read(socket, boost::asio::buffer(respond));
            std::cout << "Server: " << respond << std::endl;
        }


    }
    catch (std::exception& e) {

    }

    return 0;
}

//int main() {
//
//
//
//
//
//
//    char socketAddress1[] = "102a.15.12.22:205";
//
//    char socketAddress2[] = "102.15.12.22:205";
//
//    std::string ip;
//    int port;
//
//    try {
//
//        ip = networkPart(socketAddress2, "ip");
//        port = std::stoi(networkPart(socketAddress2, "port"));
//    }
//    catch (const std::invalid_argument& e) {
//        std::cout << "[Invalid_argument ERROR]: " << e.what() << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cout << "[Invalid_argument ERROR]: " << e.what() << std::endl;
//    }
//
//    std::cout << "ip: " <<  ip << std::endl;
//    std::cout << "port: " <<  port << std::endl;
//    
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//  //  //unit tests:
//
//  //  //checkSocketForm(port, "port")
//  //  //for (int i = 0; i < 1000000; i++) {
//  //  //    port = std::to_string(i);
//  //  //    bool rightPort = checkSocketForm(port, "port");
//
//  //  //    if (!rightPort) {
//  //  //        std::cout << "Illegal port: " << port << std::endl;
//  //  //        break;
//  //  //    }
//  //  //}
//  //  //for (int i = -5; i < 0; i++) {
//  //  //    port = std::to_string(i);
//  //  //    bool rightPort = checkSocketForm(port, "port");
//
//  //  //    if (!rightPort) {
//  //  //        std::cout << "Illegal port: " << port << std::endl;
//  //  //        break;
//  //  //    }
//  //  //}
//
//
//  //  //checkSocketForm(int, "int")
//  //  int max = 260;
//  //  int min = 255;
//
//  //  //for (int i = min; i < max; i++) {
//
//  //  //    std::string firstSegment = std::to_string(i);
//
//  //  //    for (int j = min; j < max; j++) {
//  //  //        std::string secondSegment = std::to_string(j);
//  //  //        for (int k = min; k < max; k++) {
//  //  //            std::string thirdSegment = std::to_string(k);
//
//  //  //            for (int p = min; p < max; p++) {
//  //  //                std::string fourthSegment = std::to_string(p);
//
//  //  //                std::string ip = format("{}.{}.{}.{}", firstSegment, secondSegment, thirdSegment, fourthSegment);
//  //  //                bool rightIP = checkSocketForm(ip, "ip");
//
//  //  //                std::cout << "Here 1:" << i << j << k << p << std::endl;
//  //  //                if (!rightIP) {
//  //  //                    std::cout << "Here 2:" << i << j << k << p << std::endl;
//  //  //                    std::cout << "Illegal ip: " << ip << std::endl;
//  //  //                    break;
//  //  //                }
//  //  //                else {
//  //  //                    std::cout << "Here 3:" << std::endl;
//  //  //                }
//  //  //            }
//  //  //        }
//  //  //    }
//  //  //}
//
//    std::cin.get();
//}
//

