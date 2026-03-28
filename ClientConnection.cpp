/*
    Client application
*/
#include "ClientConnection.h"

using boost::asio::ip::tcp;
ClientMenu menu;

uint8_t version = 2;
std::vector<int> legalCodes = { 2100, 2101, 2102, 2103, 2104, 9000};

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
        boost::asio::connect(socket, resolver.resolve(ip, port));


        std::cout << "Type Username: ";

        std::string userTextUsername = "";
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

        ResponseHandler serverResponse(0);
        uint8_t messageType = 255; // For request 703

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
                    else if (legal_option == "client list")
                        code = 701;
                    else if (legal_option == "public key")
                        code = 702;
                    else if (legal_option == "request symmetric key")
                    {
                        code = 703;
                        messageType = 1;

                    }
                    else if (legal_option == "send symmetric key")
                    {
                        code = 703;
                        messageType = 2;

                    }
                    else if (legal_option == "send message")
                    {
                        code = 703;
                        messageType = 3;

                    }
                    else if (legal_option == "send a file")
                    {
                        code = 703;
                        messageType = 4;

                    }
                    else if (legal_option == "pull messages") {
                        code = 704;
                    }
                    else {
                        std::cout << "Illegal request!" << std::endl;
                        continue;
                    }

                }

                /* -----------------------------------------------------Any legal code----------------------------------------------------------------------------- */
                
                RequestHandler request = (code);

                std::vector<uint8_t> buffer;

                try {
                    if (code == 700) {
                        buffer = request.HandleRequest700(userTextUsername, rsa.getPublicKey());
                    }
                    else if (code == 701) {
                        buffer = request.HandleRequest701(userTextUsername);
                    }
                    else if (code == 702) {
                        std::vector<uint8_t> targetIdBytes = general::TargetNameExists(serverResponse);

                        if (targetIdBytes.empty())
                            continue;

                        std::string targetIdHex = general::ConvertBytesToHex(targetIdBytes);
                        buffer = request.HandleRequest702(userTextUsername, targetIdHex);
                    }
                    else if (code == 703) {

                        std::vector<uint8_t> targetIdBytes = general::TargetNameExists(serverResponse);
                        if (targetIdBytes.empty())
                            continue;

                        if (messageType == 2) {
                            std::string targetIdHex = general::ConvertBytesToHex(targetIdBytes);

                            
                            std::vector<uint8_t> request702Buffer = request.HandleRequest702(userTextUsername, targetIdHex);

                            if (request702Buffer.empty()) {
                                std::cout << "[ERROR]: Failed to build request 702.\n";
                                continue;
                            }

                            boost::asio::write(socket, boost::asio::buffer(request702Buffer, request702Buffer.size()));

                            // Collect the header:
                            const uint8_t header_size = 7;
                            uint8_t response702[header_size];
                            boost::asio::read(socket, boost::asio::buffer(response702, 7));

                            /* We want to get the target's public key which the response of request 702 returns. */
                            ResponseHeader response_header_702 = ResponseHeader();
                            response_header_702.BuildResponseHeader(response702);

                            // Defensive purposes:
                            if (response_header_702.code != 2102 || response_header_702.payload_size != 176) {
                                std::cout << "[ERROR]: Expected response 2102 for request 702.\n";
                                std::vector<uint8_t> illegalPayload;
                                if (response_header_702.payload_size > 0) {
                                    illegalPayload.resize(response_header_702.payload_size);
                                    boost::asio::read(socket, boost::asio::buffer(illegalPayload, response_header_702.payload_size));
                                }
                                continue;
                            }

                            std::vector<uint8_t> payload702;
                            payload702.resize(response_header_702.payload_size);
                            boost::asio::read(socket, boost::asio::buffer(payload702, response_header_702.payload_size));

                            std::vector<uint8_t> targetData = serverResponse.HandleResponse2102(payload702);
                            if (targetData.size() != 176) {
                                std::cout << "[ERROR]: Invalid response 2102 content.\n";
                                continue;
                            }

                            std::vector<uint8_t> targetClientIdBytes;
                            std::vector<uint8_t> publicKeyBytes;

                            for (int i = 0; i < 16; i++) {
                                targetClientIdBytes.push_back(targetData.at(i));
                            }
                            for (int i = 16; i < 176; i++) {
                                publicKeyBytes.push_back(targetData.at(i));
                            }

                            // Generate/reuse symmetric key and encrypt it with target public key
                            std::vector<uint8_t> encryptedSymmetricKey = serverResponse.PrepareEncryptedSymmetricKey(targetClientIdBytes, publicKeyBytes);

                            if (encryptedSymmetricKey.empty()) {
                                std::cout << "[ERROR]: Failed to prepare encrypted symmetric key.\n";
                                continue;
                            }

                            buffer = request.HandleRequest703Type2(userTextUsername, targetIdBytes, encryptedSymmetricKey);

                        }
                        else {
                            buffer = request.HandleRequest703(userTextUsername, targetIdBytes, messageType, serverResponse);
                        }
                    }
                    else if (code == 704) {
                        buffer = request.HandleRequest704(userTextUsername);
                    }

                    if (buffer.empty())
                        continue;

                    if (buffer.size() < 23) {
                        std::cout << "[ERROR]: illegal buffer size returned: " << buffer.size() << std::endl;
                        continue;
                    }

                    boost::asio::write(socket, boost::asio::buffer(buffer, buffer.size()));

                    /* Response from the server: */
                    const uint8_t header_size = 7;
                    uint8_t response[header_size];

                    boost::asio::read(socket, boost::asio::buffer(response, 7));

                    ResponseHeader response_header = ResponseHeader();
                    response_header.BuildResponseHeader(response);
                    
                    /* ---------------------------------
                        Check if code is 9000 (failure) 
                    ---------------------------------- */

                    // Any of the options are illegal, so just define it as illegal to avoid errors and bugs
                    if (!(std::find(legalCodes.begin(), legalCodes.end(), response_header.code) != legalCodes.end())) {
                        std::cout << "Illegal response code received\n";
                        response_header.code = 9000;
                    }

                    /* ----------------------------------------------------------------------------------
                        or 2100 (success for request = 700) in order to work with the payload's content
                    ------------------------------------------------------------------------------------ */ 
                    
                    // If the code is 2100 but the payload size is not exactly 16, then reject and define code as 9000 to avoid errors and bugs
                    if (response_header.code == 2100 && response_header.payload_size != 16) {
                        std::cout << "Illegal payload size for response 2100 received\n";
                        response_header.code = 9000;
                    }

                    /* ----------------------------------------------------------------------------------
                        or 2101 (success for request = 701) in order to work with the payload's content
                    ------------------------------------------------------------------------------------ */

                    // If the code is 2101 but payload's size is not divided by 271, then payload is illegal (each record is 271 bytes)
                    else if (response_header.code == 2101 && response_header.payload_size % 271 != 0) {
                        std::cout << "Illegal payload size for response 2101 received\n";
                        response_header.code = 9000;
                    }

                    /* ----------------------------------------------------------------------------------
                        or 2102 (success for request = 702) in order to work with the payload's content
                    ------------------------------------------------------------------------------------ */

                    // If the code is 2102 but payload's size is not 176 ( 16 bytes for Client ID, 160 for public key)
                    else if (response_header.code == 2102 && response_header.payload_size != 176) {
                        std::cout << "Illegal payload size for response 2102 received\n";
                        response_header.code = 9000;
                    }

                     /* ----------------------------------------------------------------------------------
                        or 2103 (success for request = 703) in order to work with the payload's content
                    ------------------------------------------------------------------------------------ */

                    // If the code is 2103 but payload's size is not 176 ( 16 bytes for Client ID, 160 for public key)
                    else if (response_header.code == 2103 && response_header.payload_size != 20) {
                        std::cout << "Illegal payload size for response 2103 received\n";
                        response_header.code = 9000;
                    }

                    
                    
                    
                    //Useless for now:
                    /* ----------------------------------------------------------------------------------
                        or 2104 (success for request = 704) in order to work with the payload's content
                    ------------------------------------------------------------------------------------ */
                    //// If the code is 2104 but payload's size is not 176 ( 16 bytes for Client ID, 160 for public key)
                    //else if (response_header.code == 2103 && response_header.payload_size != 20) {
                    //    std::cout << "Illegal payload size for response 2103 received\n";
                    //    response_header.code = 9000;
                    //}


                    std::vector<uint8_t> payload;
                    //code = response_header.code;

                    if (response_header.code == 9000) { //Rejection -> payload = 0
                        serverResponse.HandleResponse9000(payload);

                    }
                    else if (response_header.code == 2100) {


                        payload.resize(16);
                        boost::asio::read(socket, boost::asio::buffer(payload, 16));

                        std::string uniqueIdentifier = serverResponse.HandleResponse2100(payload);

                        std::string privateKey = rsa.getPrivateKey();

                        OpenFolderInsideExeFile("Users");
                        bool fileCreated = CreateUserInfoFile(userTextUsername, uniqueIdentifier, privateKey);

                        if(!fileCreated)
                            throw std::runtime_error("Register succeeded on server, but failed to save user info locally.\n");


                    }
                    else if (response_header.code == 2101) {
                        payload.resize(response_header.payload_size);
                        boost::asio::read(socket, boost::asio::buffer(payload, response_header.payload_size));

                        serverResponse.HandleResponse2101(payload, userTextUsername);
                    }
                    else if (response_header.code == 2102) {
                        payload.resize(response_header.payload_size);
                        boost::asio::read(socket, boost::asio::buffer(payload, response_header.payload_size));

                        std::vector<uint8_t> response = serverResponse.HandleResponse2102(payload);

                        // Create clientID and publicKey outputs as requested
                        std::vector<uint8_t> clientIdBytes;
                        std::vector<uint8_t> publicKeyBytes;
                        for (size_t i = 0; i < 16; i++) {
                            clientIdBytes.push_back(payload.at(i));
                        }
                        for (size_t i = 16; i < payload.size(); i++) {
                            publicKeyBytes.push_back(payload.at(i));
                        }
                        std::string ClientId = general::ConvertBytesToHex(clientIdBytes);
                        std::string publicKey = general::ConvertBytesToHex(publicKeyBytes);

                        std::cout << "Target's Client ID: " << ClientId << std::endl;
                        std::cout << "Target's Public-Key: " << publicKey << std::endl;
                        std::cout << std::endl;
                    }
                    else if (response_header.code == 2103) {
    
                        payload.resize(response_header.payload_size);
                        boost::asio::read(socket, boost::asio::buffer(payload, response_header.payload_size));

                        serverResponse.HandleResponse2103(payload);
                    }
                    else if (response_header.code == 2104) {

                        payload.resize(response_header.payload_size);
                        boost::asio::read(socket, boost::asio::buffer(payload, response_header.payload_size));

                        size_t i = 0;

                        while (i < payload.size()) {

                            // We need at least 25 bytes first:
                            // fromId [16] + messageID [4] + messageType [1] + messageSize [4]
                            if (payload.size() - i < 25) {
                                std::cout << "Invalid 2104 payload!" << std::endl;
                                break;
                            }

                            // We will break the messages with 25 bytes first (the 2104 code structure) and then the content itself which could change
                            std::vector<uint8_t> fromId(payload.begin() + i, payload.begin() + i + 16);
                            i += 16;

                            std::vector<uint8_t> messageID(payload.begin() + i, payload.begin() + i + 4);
                            i += 4;

                            uint8_t messageType = payload[i];
                            i += 1;
                             
                            // Convert to little endian 
                            uint32_t messageSize =
                                static_cast<uint32_t>(payload[i]) |
                                (static_cast<uint32_t>(payload[i + 1]) << 8) |
                                (static_cast<uint32_t>(payload[i + 2]) << 16) |
                                (static_cast<uint32_t>(payload[i + 3]) << 24);
                            i += 4;

                            if (payload.size() - i < messageSize) {
                                std::cout << "Invalid message size in 2104!" << std::endl;
                                break;
                            }

                            std::vector<uint8_t> content(payload.begin() + i, payload.begin() + i + messageSize);
                            i += messageSize;

                            if (messageType == 1) {
                                bool exists = false; // Tracks if the user was found in the members list
                                std::string name = "";
                                name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                
                                if (name != "") {
                                    std::cout << "Symmetric key request received from: " << name << std::endl;
                                    exists = true;
                                }
                                // Could be a scenario that the user got deleted after sending the request...
                                if (!exists) {
                                    std::cout << "Symmetric key request received from a not existing user..." << std::endl;
                                    std::cout << "Maybe the user was deleted after sending the request?" << std::endl;
                                }

                                
                            }
                            else if (messageType == 2) {
                                // Store the ID and symmetric key
                                std::string targetID = general::ConvertBytesToHex(fromId);

                                std::string name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                bool succeed = serverResponse.DecryptAndSaveSymmetricKey(userTextUsername, content, targetID, serverResponse);
                                if (succeed) {
                                    std::string name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                    std::cout << "Symmetric key sent from " << name << " is saved!" << std::endl;
                                }
                                else {
                                    std::cout << "Failed to save symmetric key!" << std::endl;
                                    continue;
                                }
                            }
                            // Get a message, decrypt and print it
                            else if (messageType == 3) {
                                // Decrypt the message with symmetric key
                                // print message
                                std::string targetID = general::ConvertBytesToHex(fromId);
                                std::string decryptedMessage = serverResponse.DecryptMessage(content, targetID, serverResponse);
                                std::string name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                std::cout << "[" << name << " Message]: " << decryptedMessage << std::endl;
                            }
                            // Get a file, decrypt, print and store it 
                            else if (messageType == 4) {

                                std::string targetID = general::ConvertBytesToHex(fromId);
                                std::string decryptedMessage = serverResponse.DecryptMessage(content, targetID, serverResponse);

                                // The file will be saved inside the EXE file (where client.exe stored)
                                std::filesystem::path exePath = FindWhereEXEFileIs();
                                std::filesystem::path path = RemoveFilenameAndSeparator(exePath, "Received_Files");
                                OpenFolderInsideExeFile("Received_Files");

                                path += "/file_from_" + targetID + ".bin";
                                bool exists = CheckIfFileAlreadyExists(path);

                                if (!exists) {
                                    std::ofstream file(path, std::ios::binary);
                                    file.write(decryptedMessage.data(), decryptedMessage.size());
                                    file.close();

                                    std::string name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                    std::cout << name << " sent you a file, located at:\n";
                                    std::cout << path << std::endl;
                                }
                                else {
                                    std::string name = serverResponse.GetANameFromMembersList(serverResponse, fromId);
                                    std::cout << name << " tried to send you a file, but file with identical name already exists!" << std::endl;
                                }  
                             
                            }
                        }
                    }
                    else {
                        std::cout << "Something gone wrong with the response! Try again" << std::endl;
                        continue;
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

