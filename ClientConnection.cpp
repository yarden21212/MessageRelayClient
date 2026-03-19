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
#include "FileFunctionality.h"

#include "CustomExceptions.h"

#include <rsa.h>
#include "RSAWrapper.h"

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
        RSAPrivateWrapper rsa;
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        //boost::asio::connect(socket, resolver.resolve(argv[1], argv[2]));
        boost::asio::connect(socket, resolver.resolve(ip, port));


        std::cout << "Type Username: ";

        std::string userTextUsername;
        std::getline(std::cin, userTextUsername);
        userTextUsername = general::reduce(userTextUsername);

        while (userTextUsername.empty()) {
            std::cout << "Illegal username given, please enter at least 1 character\nType Username: ";
            std::getline(std::cin, userTextUsername);
            userTextUsername = general::reduce(userTextUsername);
        }
    
        if (!general::ValidateUsername(userTextUsername, 255)) {
            std::cout << "Username has to be at max 255 characters.\nProgram terminated!\n";
            std::exit(1);
        }


        for (;;) {
            std::cout << std::endl;

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

                try {
                    if (code == 700) {
                        buffer = request.HandleRequest700(userTextUsername, client_id);
                    }


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
                    
                    /* ------------------------------------------------------------------------------------------------
                        Check if code is 9000 (failure) or 2100 (success) in order to work with the payload's content
                    ------------------------------------------------------------------------------------------------- */

                    // Any of the options are illegal, so just define it as illegal to avoid errors and bugs
                    if (response_header.code != 9000 && response_header.code != 2100) {
                        std::cout << "Illegal response code received\n";
                        response_header.code = 9000;
                    }
                    // If the code is 2100 but the payload size is not exactly 16, then reject and define code as 9000 to avoid errors and bugs
                    if (response_header.code == 2100 && response_header.payload_size != 16) {
                        std::cout << "Illegal payload size for response 2100 received\n";
                        response_header.code = 9000;
                    }

                    std::vector<uint8_t> payload;
                    code = response_header.code;
                    ResponseHandler serverResponse = (code);

                    if (code == 9000) { //Rejection -> payload = 0
                        serverResponse.HandleResponse9000(payload);

                    }
                    else if (code == 2100) {


                        payload.resize(16);
                        boost::asio::read(socket, boost::asio::buffer(payload, 16));

                        std::cout << "payload size: " << payload.size() << std::endl;

                        std::string uniqueIdentifier = serverResponse.HandleResponse2100(payload);
                        std::cout << "uniqueIdentifier: " << uniqueIdentifier << std::endl;

                        std::string privateKey = rsa.getPrivateKey();

                        OpenFolderInsideExeFile("Users");
                        bool fileCreated = CreateUserInfoFile(userTextUsername, uniqueIdentifier, privateKey);

                        if(!fileCreated)
                            throw std::runtime_error("Register succeeded on server, but failed to save user info locally.\n");


                    }
                    else {
                        std::cout << "I'm outside..." << std::endl;
                    }
                }
                catch (const IllegalSizeException& e) {
                    std::cout << "I'm here!!" << std::endl;
                    std::cout << e.what() << std::endl;
                    continue;
                }
                catch (const struct boost::wrapexcept<class boost::system::system_error>& e) {
                    std::cout << "\n[ERROR]:\nThe server forcibly closed!" << std::endl;
                    std::cout << "Expanded version: " << e.what() << std::endl;
                    std::cout << "Program terminates..." << std::endl;
                    break;
                }
                catch (const std::runtime_error& e) {
                    std::cout << e.what() << std::endl;
                    std::cout << "Program terminates..." << std::endl;
                    break;
                }
                catch (const std::exception& e) {
                    std::cout << "I'm here!!2" << std::endl;
                    std::cout << e.what() << std::endl;
                    std::cout << "Type:    " << typeid(e).name() << "\n";
                    std::cout << "Program terminates..." << std::endl;
                    break;
                }
                
               
            }

        }

    }
    catch ([[ maybe_unused ]] std::exception& e) {
        std::cout << "[ERROR:]" << e.what() << std::endl;
    }

    return 0;
}

