#include "RequestHandler.h"

RequestHandler::RequestHandler(uint16_t code) {
    this->code = code;
}
std::vector<uint8_t> RequestHandler::HandleRequest700(std::string &username, const std::string& publicKey) {

    if (!general::ValidatePublicKey(publicKey, 160))
    {
        throw IllegalSizeException("[ERROR]: Public key has to be 160 characters. Try again!");   
    }

    uint16_t code = 700;

    std::string userTextUsernamePadding;
    for (size_t i = 0; i < 255 - username.size(); i++) {
        userTextUsernamePadding += '\0';
    }


    std::string publicKeyPadding;
    for (size_t i = 0; i < 160 - publicKey.size(); i++) {
        publicKeyPadding += '\0';
    }

    int payloadSize = username.size() + userTextUsernamePadding.size()
        + publicKey.size() + publicKeyPadding.size();

    uint8_t zero_client_id[16] = { 0 };
    RequestHeader header(zero_client_id, RequestHandler::VERSION, code, payloadSize);

    std::vector<uint8_t> buffer;
    buffer.resize(sizeof(header) + username.size()
        + userTextUsernamePadding.size()
        + publicKey.size()
        + publicKeyPadding.size());

    int currSize = 0;

    /* Create new version of the header for supporting every platform -> Will support little-endian*/

    // version [1 byte] -> doesn't really need
    uint8_t versionToLittleEndian = header.version;
    // code [2 bytes]
    uint8_t code_to_little_endian[2];
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);

    // payload_size [4 bytes]
    uint8_t payload_size_to_little_endian[4];
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);

    // Copying the data (including little-endian) to the buffer: [header][payload] -> [header][user name][public key]
    std::memcpy(buffer.data(), header.client_id, sizeof(header.client_id));
    currSize += sizeof(header.client_id);

    std::memcpy(buffer.data() + currSize, &versionToLittleEndian, sizeof(versionToLittleEndian));
    currSize += sizeof(versionToLittleEndian);

    std::memcpy(buffer.data() + currSize, code_to_little_endian, sizeof(code_to_little_endian));
    currSize += sizeof(code_to_little_endian);

    std::memcpy(buffer.data() + currSize, payload_size_to_little_endian, sizeof(payload_size_to_little_endian));
    currSize += sizeof(payload_size_to_little_endian);

    std::memcpy(buffer.data() + currSize, username.data(), username.size());
    currSize += username.size();

    std::memcpy(buffer.data() + currSize, userTextUsernamePadding.data(), userTextUsernamePadding.size());
    currSize += userTextUsernamePadding.size();

    std::memcpy(buffer.data() + currSize, publicKey.data(), publicKey.size());
    currSize += publicKey.size();

    std::memcpy(buffer.data() + currSize, publicKeyPadding.data(), publicKeyPadding.size());

    return buffer;
}

std::vector<uint8_t> RequestHandler::HandleRequest701(std::string& username) {

    uint16_t code = 701;
    
    std::string clientIdHex = general::getUserId(username);
    if (clientIdHex.empty()) { // The user doesn't exist locally
        return {};
    }

    std::vector<uint8_t> hexToBytes = general::ConvertHexToBytes(clientIdHex);
    int payloadSize = 0;

    RequestHeader header(hexToBytes.data(), RequestHandler::VERSION, this->code, 0);
    std::vector<uint8_t> buffer;
    buffer.resize(sizeof(header));


    int currSize = 0;

    /* Create new version of the header for supporting every platform -> Will support little-endian*/
    uint8_t versionToLittleEndian = header.version; // version 
    uint8_t code_to_little_endian[2]; // code 
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);
    uint8_t payload_size_to_little_endian[4]; // payload_size
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);

    // Copying the data (including little-endian) to the buffer: [header][payload] -> [header][user name][public key]
    std::memcpy(buffer.data(), header.client_id, sizeof(header.client_id));
    currSize += sizeof(header.client_id);

    std::memcpy(buffer.data() + currSize, &versionToLittleEndian, sizeof(versionToLittleEndian));
    currSize += sizeof(versionToLittleEndian);

    std::memcpy(buffer.data() + currSize, code_to_little_endian, sizeof(code_to_little_endian));
    currSize += sizeof(code_to_little_endian);

    std::memcpy(buffer.data() + currSize, payload_size_to_little_endian, sizeof(payload_size_to_little_endian));
    currSize += sizeof(payload_size_to_little_endian);

    return buffer;
}

/* Retrieving a client's public key */
std::vector<uint8_t> RequestHandler::HandleRequest702(std::string& username, const std::string& targetIdHex) {

    uint16_t code = 702;

    std::string clientIdHex = general::getUserId(username);
    if (clientIdHex.empty()) { // The user doesn't exist locally
        return {};
    }

    std::vector<uint8_t> clientId = general::ConvertHexToBytes(clientIdHex);
    std::vector<uint8_t> targetId = general::ConvertHexToBytes(targetIdHex);
    const size_t payloadSize = 16;

    if (clientId.size() != payloadSize || targetId.size() != payloadSize) {
        return {};
    }

    const size_t headerSize = 23;
    RequestHeader header(clientId.data(), RequestHandler::VERSION, code, payloadSize);
    std::vector<uint8_t> payload = targetId;

    std::vector<uint8_t> buffer;
    buffer.resize(headerSize + payload.size());


    int currSize = 0;

    /* Create new version of the header for supporting every platform -> Will support little-endian*/
    uint8_t versionToLittleEndian = header.version; // version 
    uint8_t code_to_little_endian[2]; // code 
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);
    uint8_t payload_size_to_little_endian[4]; // payload_size
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);

    // Copying the data (including little-endian) to the buffer: [header][payload] -> [header][user name][public key]
    std::memcpy(buffer.data(), header.client_id, sizeof(header.client_id));
    currSize += sizeof(header.client_id);

    std::memcpy(buffer.data() + currSize, &versionToLittleEndian, sizeof(versionToLittleEndian));
    currSize += sizeof(versionToLittleEndian);

    std::memcpy(buffer.data() + currSize, code_to_little_endian, sizeof(code_to_little_endian));
    currSize += sizeof(code_to_little_endian);

    std::memcpy(buffer.data() + currSize, payload_size_to_little_endian, sizeof(payload_size_to_little_endian));
    currSize += sizeof(payload_size_to_little_endian);

    std::memcpy(buffer.data() + currSize, payload.data(), payload.size());
    currSize += payload.size();

    return buffer;
}

//The implementations of those functions are in the file's botton
size_t AddHeaderToBuffer(std::vector<uint8_t>* buffer, size_t targetIdSize, size_t messageTypeSize, size_t contentSizeFieldSize, uint32_t actualContentSize, std::vector<uint8_t> clientId
    , uint32_t payloadSize);
bool Check703Legality(std::string& username, const std::vector<uint8_t> targetIdBytes, const uint8_t messageType);

/*
    Handle request 703 (Send message to a client).
    All of the different message types will be handled here.
    We will use set and set.find() for logarithmic complexity: https://en.cppreference.com/w/cpp/container/set/find
*/
std::vector<uint8_t> RequestHandler::HandleRequest703(std::string& username, const std::vector<uint8_t> targetIdBytes, const uint8_t messageType, ResponseHandler& serverResponse) {

    uint16_t code = 703;

    bool is703Legal = Check703Legality(username, targetIdBytes, messageType);
    if (!is703Legal) // The call is legal, the basic configuration of this call is illegal
        return {};

    std::string clientIdHex = general::getUserId(username);

    std::vector<uint8_t> clientId = general::ConvertHexToBytes(clientIdHex);
    std::vector<uint8_t> targetId = targetIdBytes;
    std::vector<uint8_t> buffer;

    /* Creation of payload: */
    size_t targetIdSize = 16;
    size_t messageTypeSize = 1;
    size_t contentSizeFieldSize = 4;


    /* Now we need to check which messageType was chosen: */
    // Request for a symmetric-key
    if (messageType == 1) {

        uint32_t actualContentSize = 0;
        uint32_t payloadSize = targetIdSize + messageTypeSize + contentSizeFieldSize + actualContentSize;

        // Creation of header:
        size_t currSize = AddHeaderToBuffer(&buffer, targetIdSize, messageTypeSize, contentSizeFieldSize, actualContentSize, clientId, payloadSize);

        uint8_t content_size_to_little_endian[4];
        general::ToLittleEndianFromUint32(content_size_to_little_endian, actualContentSize);

        std::memcpy(buffer.data() + currSize, targetIdBytes.data(), targetIdSize);
        currSize += targetIdSize;

        std::memcpy(buffer.data() + currSize, &messageType, messageTypeSize);
        currSize += messageTypeSize;

        std::memcpy(buffer.data() + currSize, content_size_to_little_endian, contentSizeFieldSize);
        currSize += contentSizeFieldSize;

        return buffer;
    }
    // User wants to send symmetric key
    else if (messageType == 2) {
        std::cout << "[ERROR]: Request 152 needs the encrypted symmetric key content first.\n";
        std::cout << "You must fetch the target public key with 702 before building 703 type 2.\n";
        return {};
    }
    // User sends a text message (which will be encrypted by symetric-key)
    else if (messageType == 3) {
        std::cout << "Type message: ";
        std::string textMessage;
        std::getline(std::cin >> std::ws, textMessage);
        general::reduce(textMessage); // Deletes unnecessary white-spaces between words. For example: "    Hello my\t   \tfriend\t\t\t  " -> "Hello my friend"  


        // Check if symmetric key already exists with the given target
        std::vector<uint8_t> symmetricKey;
        for (auto& member : serverResponse.getMembersList()) {
            if (member.id == targetIdBytes) {
                symmetricKey = member.symmetricKey;

                break;
            }
        }

        if (symmetricKey.empty()) {
            std::cout << "[ERROR]: No symmetric key exists with this client.\n";
            return {};
        }

        // Encrypt the message  
        AESWrapper aes(symmetricKey.data(), 16);
        std::string messageEncrypted = aes.encrypt(textMessage.data(), textMessage.size());

        uint32_t encryptedContentSize = messageEncrypted.size();

        uint32_t payloadSize = targetIdSize + messageTypeSize + contentSizeFieldSize + encryptedContentSize;

        //Creation of header:
        size_t currSize = AddHeaderToBuffer(&buffer, targetIdSize, messageTypeSize, contentSizeFieldSize, encryptedContentSize, clientId, payloadSize);

        uint8_t content_size_to_little_endian[4];
        general::ToLittleEndianFromUint32(content_size_to_little_endian, encryptedContentSize);


        // Payload (content)
        std::memcpy(buffer.data() + currSize, targetId.data(), targetIdSize);
        currSize += targetIdSize;
        std::memcpy(buffer.data() + currSize, &messageType, messageTypeSize);
        currSize += messageTypeSize;
        std::memcpy(buffer.data() + currSize, content_size_to_little_endian, contentSizeFieldSize);
        currSize += contentSizeFieldSize;
        //std::memcpy(buffer.data() + currSize, textMessage.data(), actualContentSize);
        //currSize += textMessage.size();

        std::memcpy(buffer.data() + currSize, messageEncrypted.data(), encryptedContentSize);
        currSize += encryptedContentSize;

        return buffer;
    }

    // ----------- User encrypts and send a file (bonus part) -------------
    else if (messageType == 4) {
        

        // Get the wanted file: User enter path -> get the file location
        std::cout << "Enter a file's path: ";
        std::string path;
        std::getline(std::cin >> std::ws, path);

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "[ERROR]: File doesn't exist.." << std::endl;
            return {};
        }

        std::string fileContent;
        char ch;
        while (file.get(ch))
        {
            fileContent += ch;
        }
        file.close();
        
        // Check if symmetric key already exists with the given target
        std::vector<uint8_t> symmetricKey;
        for (auto& member : serverResponse.getMembersList()) {
            if (member.id == targetIdBytes) {
                symmetricKey = member.symmetricKey;
                break;
            }
        }

        if (symmetricKey.empty()) {
            std::cout << "[ERROR]: No symmetric key exists with this client.\n";
            return {};
        }

        // Encrypt the file
        AESWrapper aes(symmetricKey.data(), 16);
        std::string FileEncrypted = aes.encrypt(fileContent.data(), fileContent.size());
        uint32_t encryptedContentSize = FileEncrypted.size();


        uint32_t payloadSize = targetIdSize + messageTypeSize + contentSizeFieldSize + encryptedContentSize;

        //Creation of header:
        size_t currSize = AddHeaderToBuffer(&buffer, targetIdSize, messageTypeSize, contentSizeFieldSize, encryptedContentSize, clientId, payloadSize);

        uint8_t content_size_to_little_endian[4];
        general::ToLittleEndianFromUint32(content_size_to_little_endian, encryptedContentSize);


        if (targetId.size() != 16) {
            std::cout << "[ERROR]: targetId must be 16 bytes.." << std::endl;
            return {};
        }

        // Payload (content)
        std::memcpy(buffer.data() + currSize, targetId.data(), targetIdSize);
        currSize += targetIdSize;
        std::memcpy(buffer.data() + currSize, &messageType, messageTypeSize);
        currSize += messageTypeSize;
        std::memcpy(buffer.data() + currSize, content_size_to_little_endian, contentSizeFieldSize);
        currSize += contentSizeFieldSize;
        //std::memcpy(buffer.data() + currSize, textMessage.data(), actualContentSize);
        //currSize += textMessage.size();

        std::memcpy(buffer.data() + currSize, FileEncrypted.data(), encryptedContentSize);
        currSize += encryptedContentSize;

        return buffer;
    }
    else {
        /* else can't happen. Verified with set.find() -> Avoids warnings */
        return {};
    }

}
std::vector<uint8_t> RequestHandler::HandleRequest704(std::string userTextUsername) {
    std::string clientIdHex = general::getUserId(userTextUsername);
    if (clientIdHex.empty()) { // The user doesn't exist locally
        return {};
    }

    uint16_t code = 704;

    std::vector<uint8_t> idHexToBytes = general::ConvertHexToBytes(clientIdHex);
    std::vector<uint8_t> buffer;

    uint8_t idToBytes[16] = {};
    for (size_t i = 0; i < idHexToBytes.size(); i++) {
        idToBytes[i] = idHexToBytes[i];
    }

    //Creation of header:
    RequestHeader header(idToBytes, RequestHandler::VERSION, code, 0);

    buffer.resize(sizeof(header));

    int currSize = 0;

    // version [1 byte] -> doesn't really need
    uint8_t versionToLittleEndian = header.version;
    // code [2 bytes]
    uint8_t code_to_little_endian[2];
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);
    // payload_size [4 bytes]
    uint8_t payload_size_to_little_endian[4];
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);


    // Copying the data (including little-endian) to the buffer: [header][payload] -> [header][user name][public key]
    std::memcpy(buffer.data(), header.client_id, sizeof(header.client_id));
    currSize += sizeof(header.client_id);
    std::memcpy(buffer.data() + currSize, &versionToLittleEndian, sizeof(versionToLittleEndian));
    currSize += sizeof(versionToLittleEndian);
    std::memcpy(buffer.data() + currSize, code_to_little_endian, sizeof(code_to_little_endian));
    currSize += sizeof(code_to_little_endian);
    std::memcpy(buffer.data() + currSize, payload_size_to_little_endian, sizeof(payload_size_to_little_endian));
    currSize += sizeof(payload_size_to_little_endian);

    return buffer;
}


/* ------------------------------------------------------------------------------- Extra helper methods -------------------------------------------------------------------------------*/


/*
    Builds the 23 bytes for the header.
    This code repeats in many different places, so this method will organize the program better.
*/
size_t AddHeaderToBuffer(std::vector<uint8_t>* buffer, size_t targetIdSize, size_t messageTypeSize, size_t contentSizeFieldSize, uint32_t actualContentSize, std::vector<uint8_t> clientId
    , uint32_t payloadSize) {
    uint16_t code = 703;


    /* Add the header & payload's content into buffer */
    // Header:
    RequestHeader header(clientId.data(), RequestHandler::VERSION, code, payloadSize);
    // Convert numeric values of the header to little-endian:
    uint8_t versionToLittleEndian = header.version; // version 
    uint8_t code_to_little_endian[2]; // code 
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);
    uint8_t payload_size_to_little_endian[4];
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);

    buffer->resize(23 + payloadSize);
    size_t currSize = 0;
    //Header:
    std::memcpy(buffer->data(), clientId.data(), 16);
    currSize += 16;
    std::memcpy(buffer->data() + currSize, &versionToLittleEndian, 1);
    currSize += 1;
    std::memcpy(buffer->data() + currSize, code_to_little_endian, 2);
    currSize += 2;
    std::memcpy(buffer->data() + currSize, payload_size_to_little_endian, 4);
    currSize += 4;

    return currSize;
}

/*
    Plays a critical defensive role in the code for request 703.
    Checks different cases where the request could be illegal.
*/
bool Check703Legality(std::string& username, const std::vector<uint8_t> targetIdBytes, const uint8_t messageType) {

    // Check that the given message type is one of the 4 options available
    std::set types = { 1,2,3,4 };
    if (auto search = types.find(messageType); search != types.end()) {
        // Continue the progress
    }
    else
        return false;

    // Check if the client exists locally first, if yes, convert to bytes
    std::string clientIdHex = general::getUserId(username);
    if (clientIdHex.empty()) // The user doesn't exist locally
        return false;

    std::vector<uint8_t> clientId = general::ConvertHexToBytes(clientIdHex);
    std::vector<uint8_t> targetId = targetIdBytes;

    // Another defensive layer...
    if (clientId.size() != 16)
        return false;

    if (targetId.size() != 16)
        return false;

    return true;
}

/*
    Handle request 703 for message type 2 ("Send symmetric key").

    This request sends the symmetric AES key to another client.
    The symmetric key is already encrypted using the target client's public key
    before reaching this function.

    Final payload structure:
    [16B target_id][1B message_type=2][4B content_size][encrypted_symmetric_key]
*/
std::vector<uint8_t> RequestHandler::HandleRequest703Type2(std::string& username, const std::vector<uint8_t>& targetIdBytes, const std::vector<uint8_t>& encryptedSymmetricKey) {

    uint16_t code = 703;

    
    bool is703Legal = Check703Legality(username, targetIdBytes, 2);
    if (!is703Legal)
        return {};

    std::string clientIdHex = general::getUserId(username);
    std::vector<uint8_t> clientId = general::ConvertHexToBytes(clientIdHex);
    std::vector<uint8_t> targetId = targetIdBytes;
    std::vector<uint8_t> buffer;

    size_t targetIdSize = 16;
    size_t messageTypeSize = 1;
    size_t contentSizeFieldSize = 4;
    uint8_t messageType = 2;

    uint32_t actualContentSize = static_cast<uint32_t>(encryptedSymmetricKey.size());
    uint32_t payloadSize = static_cast<uint32_t>(targetIdSize + messageTypeSize + contentSizeFieldSize + actualContentSize);

    size_t currSize = AddHeaderToBuffer(&buffer, targetIdSize, messageTypeSize, contentSizeFieldSize, actualContentSize, clientId, payloadSize);

    uint8_t content_size_to_little_endian[4];
    general::ToLittleEndianFromUint32(content_size_to_little_endian, actualContentSize);

    std::memcpy(buffer.data() + currSize, targetId.data(), targetIdSize);
    currSize += targetIdSize;

    std::memcpy(buffer.data() + currSize, &messageType, messageTypeSize);
    currSize += messageTypeSize;

    std::memcpy(buffer.data() + currSize, content_size_to_little_endian, contentSizeFieldSize);
    currSize += contentSizeFieldSize;

    std::memcpy(buffer.data() + currSize, encryptedSymmetricKey.data(), encryptedSymmetricKey.size());
    currSize += encryptedSymmetricKey.size();

    std::cout << "buffer size: " << buffer.size() << std::endl;
    std::cout << "currSize: " << currSize << std::endl;

    return buffer;
}

//int main() {
//
//    //std::string hexValue = "32eb9d5205ae4868bc5acd9c755fc091";
//    //std::vector<uint8_t> byteValue = general::ConvertHexToBytes(hexValue);
//    //std::cout << "Hex representation: " << hexValue << std::endl;
//    //std::cout << "Byte representation: ";
//    //std::cout << "Size: " << byteValue.size() << std::endl;
//    //for (uint8_t byte : byteValue) {
//    //    std::cout << std::hex << std::setw(2) << std::setfill('0') << int(byte) << " ";
//    //}   
//    //std::cout << "\n";
//
//    std::string name = "Yarden";
//    RequestHandler handler = (0);
//    std::vector<uint8_t> result = handler.HandleRequest704(name);
//
//
//
//    std::system("pause");
//}
//
//

