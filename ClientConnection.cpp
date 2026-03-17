/*
    Client application
*/
#include "NetworkBelonging.h"
#include "ClientConnection.h"
#include "ClientMenu.h";
#include "GeneralFunctions.h"
#include "Request.h"
#include <vector>

#include "RequestHandler.h"
#include "ResponseHandler.h"

using boost::asio::ip::tcp;
ClientMenu menu;

uint8_t version = 2;

//void CreateBuffer(RequestHeader header, std::string_view user_text) {
//    
//
//
//}

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

        //std::string option;
        for (;;) {
            std::cout << "I'm here!" << std::endl;
            menu.DisplayOptions();

            std::string legal_option = menu.OptionSelected(menu);
            uint16_t code;

            if (legal_option != "bad input"){
                if (legal_option == "exit")
                {
                    code = 0;
                    std::exit(0);
                }
                else {
                    if (legal_option == "register")
                        code = 700; // Code = 700 -> server_request = 700 -> register a new user   
                }

                /* -----------------------------------------------------Any legal code----------------------------------------------------------------------------- */
                RequestHandler request = (code);
                std::cout << "debug - code is:" << code << std::endl;
                
                uint8_t client_id[17] = "1H76$GHML98#JS21"; // -> This will be replaced by real client_id

                std::vector<uint8_t> buffer;
                if (code == 700) {
                    buffer = request.HandleRequest700(client_id);                  
                }

                //std::cout << "buffer's content: ";
                //for (auto item : buffer) {
                //    std::cout << unsigned(item);
                //}
                //std::cout << std::endl;


                /* Delete!!!*/
                std::cout << "HEADER BYTES: ";
                for (int i = 0; i < 23; i++) {
                    std::cout << unsigned(buffer[i]) << ' ';
                }
                std::cout << std::endl;



                boost::asio::write(socket, boost::asio::buffer(buffer, max_length));

                const uint8_t header_size = 7;
                uint8_t response[header_size];
                std::cout << "debug1: header response:" << std::endl;
                boost::asio::read(socket, boost::asio::buffer(response, 7));


                std::cout << "response bytes: ";
                for (int i = 0; i < 7; i++) {
                    std::cout << unsigned(response[i]) << ' ';
                }
                std::cout << std::endl;

                
                ResponseHeader response_header = ResponseHeader();
                response_header.BuildResponseHeader(response);
                //uint8_t version = response[0] * 1;
                //uint16_t code = response[2] * 256 + response[1] * 1;
                //uint32_t payload_size =  response[6] * 256 * 256 * 256 + response[5] * 256 * 256 + response[4] * 256 + response[3];
                //std::cout << "version: " << unsigned(version) << std::endl; // Without cast to "unsigned", the value of unit8_t won't be printed
                //std::cout << "code: " << code << std::endl;
                //std::cout << "payload_size: " << payload_size << std::endl;

                
                /* ------------------------------------------------------------------------------------------------
                    Check if code is 9000 (failure) or 2100 (success)in order to work with the payload's content 
                ------------------------------------------------------------------------------------------------- */ 

                std::vector<uint8_t> payload;
                code = response_header.code;
                ResponseHandler serverResponse = (code);

                if (code == 9000) { //Rejection -> payload = 0
                    serverResponse.HandleResponse9000(payload);

                }
                else if(code == 2100){
                    payload.resize(16);
                    boost::asio::read(socket, boost::asio::buffer(payload, 16));
                    serverResponse.HandleResponse2100(payload);
                }
                else {
                    std::cout << "I'm outside..." << std::endl;
                }
               
            }

        }

    }
    catch ([[ maybe_unused ]] std::exception& e) {
        std::cout << "[ERROR:]" << e.what() << std::endl;
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

