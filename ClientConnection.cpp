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
     /*           RequestHandler request = (code);*/
                std::vector<uint8_t> buffer;
                std::cout << "debug - code is:" << code << std::endl;
                
                if (code == 700) {

                    //request.HandleRequest700(code);

                    std::cout << "Type the temporary text you want to send to the server:" << std::endl;
                    char user_id[17] = "1H76$GHML98#JS21";
                    char* id_pointer = user_id;

                    uint8_t client_id[16];
                    for (int i = 0; i < 16; i++) {
                        std::cout << "[" << i << "] = " << *id_pointer << std::endl;
                        client_id[i] = *id_pointer++;
                    }

                    // User input:
                    std::string user_text_username;
                    std::getline(std::cin, user_text_username);
                    std::string user_text_username_padding;
                    for (int i = 0; i < 255 - user_text_username.size(); i++) {
                        user_text_username_padding += '\0';
                    }
                    std::cout << "debug - user_txt is: " << user_text_username << std::endl;

                    // User public key:
                    std::string public_key = "FAKE_PUBLIC_KEY";
                    std::string public_key_padding;
                    for (int i = 0; i < 160 - public_key.size(); i++) {
                        public_key_padding += '\0';
                    }

                    int payload_size = user_text_username.size() + user_text_username_padding.size() + public_key.size() + public_key_padding.size();
                    RequestHeader header = RequestHeader(client_id, version, code, payload_size);

                    std::cout << "sizeof(header):" << sizeof(header) << "user_text_username.size():" << user_text_username.size() << std::endl;
                    std::cout << "user_text_username_padding.size(): " << user_text_username_padding.size() << "public_key.size():" << public_key.size() << std::endl;
                    std::cout << "public_key_padding.size(): " << public_key_padding.size() << std::endl;

                    buffer.resize(sizeof(header) + user_text_username.size() + user_text_username_padding.size() + public_key.size() + public_key_padding.size());
                    std::cout << "buffer size = " << buffer.size() << std::endl;
                    std::cout << "header size = " << sizeof(header) << std::endl;

                    std::cout << "paylaod size = " << payload_size << std::endl;



                    int curr_size = 0;

                    std::memcpy(buffer.data(), &header, sizeof(header));
                    curr_size = sizeof(header);
                    std::memcpy(buffer.data() + curr_size, user_text_username.data(), user_text_username.size());
                    curr_size += user_text_username.size();
                    std::memcpy(buffer.data() + curr_size, user_text_username_padding.data(), user_text_username_padding.size());
                    curr_size += user_text_username_padding.size();
                    std::memcpy(buffer.data() + curr_size, public_key.data(), public_key.size());
                    curr_size += public_key.size();
                    std::memcpy(buffer.data() + curr_size, public_key_padding.data(), public_key_padding.size());
                    
                }

                for (auto item : buffer) {
                    std::cout << item << std::endl;
                }


                boost::asio::write(socket, boost::asio::buffer(buffer, max_length));

                const uint8_t header_size = 7;
                uint8_t response[header_size];
                std::cout << "debug1: header response:" << std::endl;
                boost::asio::read(socket, boost::asio::buffer(response, 7));
                std::cout << "response is: " << response << std::endl;
                
                uint8_t version = response[0] * 1;
                uint16_t code = response[2] * 256 + response[1] * 1;
                uint32_t payload_size =  response[6] * 256 * 256 * 256 + response[5] * 256 * 256 + response[4] * 256 + response[3];
                std::cout << "version: " << unsigned(version) << std::endl; // Without cast to "unsigned", the value of unit8_t won't be printed
                std::cout << "code: " << code << std::endl;
                std::cout << "payload_size: " << payload_size << std::endl;
                
                std::vector<uint8_t> payload;
                //std::string payload;
                // Check if code is 9000 (failure) or 2100 (success) in order to work with the payload's content
                if (code == 9000) { //Rejection -> payload = 0
                    payload.resize(0);
                    std::cout << "Response:\nResponse code:" << code << std::endl;
                    std::cout << "User already exists or invalid input sent!:\n" << std::endl;

                }
                else if(code == 2100){
                    payload.resize(16);
                    boost::asio::read(socket, boost::asio::buffer(payload, 16));
                    std::cout << "Debug:" << payload.data() << std::endl;
                    std::cout << "Response:\nResponse code:" << code << std::endl;

                    std::string server_response(payload.begin(), payload.end());
                    std::cout << "Server response: " << server_response << std::endl;

   /*                 std::cout << "Payload bytes: ";*/
                    std::ostringstream x;
                    x << std::hex << std::setfill('0');
                    auto const& w{ std::setw(2) };
                    for (auto b : payload)
                        x << w << int(b);
                    std::string s{ x.str() };

                    general::separate<2,' '>(s);
                    std::cout << "Server: Client ID:" << s << std::endl;
                        
                    std::cout << std::endl;
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

