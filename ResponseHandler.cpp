#include "ResponseHandler.h"

ResponseHandler::ResponseHandler(uint16_t code) {
	this->code = code;
}

void ResponseHandler::HandleResponse9000(std::vector<uint8_t> &payload) {
	payload.resize(0);
	std::cout << "Response code:" << ResponseHandler::code << std::endl;
	std::cout << "User already exists or invalid input sent!\n" << std::endl;
}

/*
    Request 2100, it's a registration request.
    The user that asks for this operation, will be registered to the program.
    In case the user is already registered, then the request will be denied with a polite reply or code = 9000 in case it's server's case.
    The operation checks if the user is already registered locally and in the database (server will reply with code = 9000)
*/
std::string ResponseHandler::HandleResponse2100(std::vector<uint8_t>& payload) {

    std::cout << "User was created successfully." << std::endl;

    std::string uniqueIdentifierHex = general::ConvertBytesToHex(payload);
    return uniqueIdentifierHex;
}

/* 
    Handle request 2101 which asks for the list of clients who already registered to MessageU application
    The output will be decorated and will exclude the requester.
    The request will work only if the requester is also registered, if not, the request will be denied with a polite reply.
    The output will be stored behind the scenes inside an array (vector) so the user can ask for the clients' public key or do other operations with them.
*/
void ResponseHandler::HandleResponse2101(const std::vector<uint8_t>& payload, const std::string& username) {
    if (payload.size() % (16 + 255) != 0) { // Extra check -> I suppose it's not necessary because it was checked before we invoked this method
        std::cout << "Invalid payload size!" << std::endl;
        return;
    }

    int record_size = 16 + 255;
    membersList.clear(); // Clear old saved list before storing the new one

    std::cout << " " << std::endl;
    std::cout << "----------------------------------Members list----------------------------------" << std::endl;

    size_t count = 1;
    for (size_t i = 0; i < payload.size();) {

        std::cout << "\nClient " << count++ << ":" << std::endl;

        size_t  chunk = 16 + 255;
        size_t  stop = i + chunk;
        size_t  id = 0; // Counter for the id part, checks if we crossed the first 16 bytes
        std::vector<uint8_t> clientId;
        std::vector<uint8_t> clientName;
        for (; i < stop; i++) {
            if (id < 16) {
                clientId.push_back(payload[i]);

                id++;
            }
            else {
                clientName.push_back(payload[i]);
            }
        }
        // Take what we parsed, turn the name into text, and save both name + ID so we can use them later in request 702
        std::string clientNameToStr;
        for (size_t j = 0; j < 255 && clientName[j] != '\0'; j++) {
            clientNameToStr.push_back(static_cast<char>(clientName[j]));
        }

        // We don't want the user himself to be in the list.
        if (clientNameToStr == username) {
            continue;
        }

        ClientInfo info;
        info.name = clientNameToStr;
        info.id = clientId;
        membersList.push_back(info);


        std::string clientIdToHex = general::ConvertBytesToHex(clientId);
        std::cout << "   Client ID: " << clientIdToHex << std::endl;

        // Display the ID and Name of the current client:
        std::cout << "   Client Name: ";
        std::cout << clientNameToStr << std::endl;;
    }


    std::cout << "----------------------------------Members list----------------------------------" << std::endl;
    
}

std::vector<uint8_t> ResponseHandler::GetClientIdByName(const std::string& clientName) { // ADDED
    for (const auto& member : membersList) { // ADDED
        if (member.name == clientName) { // ADDED

            return member.id; // ADDED
        }
    }

    return {}; // ADDED: not found
}

/*
    Request 2102, after a user asks for the list of clients, the user can also ask for a specific client's public key which is saved inside the database.
    The request will be sent to the server which will grab the requested public-key from the DB. In case the user doesn't exist, a polite reply will be printed.
*/
std::vector<uint8_t> ResponseHandler::HandleResponse2102(const std::vector<uint8_t>& payload) {

    if (payload.size() != 16 + 160) { // Extra check (not necessary)
        std::cout << "Invalid payload size!" << std::endl;
        return {};
    }

    std::vector<uint8_t> clientIdBytes;
    for (int i = 0; i < 16; i++) {
        clientIdBytes.push_back(payload.at(i));
    }
    std::string ClientId = general::ConvertBytesToHex(clientIdBytes);

    std::vector<uint8_t> publicKeyBytes;
    for (size_t i = 16; i < payload.size(); i++) {
        publicKeyBytes.push_back(payload.at(i));
    }
    std::string publicKey = general::ConvertBytesToHex(publicKeyBytes);


    // Copying the data
    std::vector<uint8_t> buffer;
    buffer.resize(clientIdBytes.size() + publicKeyBytes.size());
    size_t currSize = 0;
    std::memcpy(buffer.data(), clientIdBytes.data(), clientIdBytes.size());
    currSize += clientIdBytes.size();
    std::memcpy(buffer.data() + currSize, publicKeyBytes.data(), publicKeyBytes.size());

    return buffer;
}

void ResponseHandler::HandleResponse2103(const std::vector<uint8_t>& payload) {
    if (payload.size() != 16 + 4) { // Extra check (not necessary, but gives an extra defense layer)
        std::cout << "Invalid payload size!" << std::endl;
        return;
    }

    std::vector<uint8_t> targetId(payload.begin(), payload.begin() + 16);
    std::vector<uint8_t> messageId(payload.begin() + 16, payload.begin() + 20);

    std::cout << "Target ID: " << general::ConvertBytesToHex(targetId) << std::endl;
    std::cout << "Message ID: " << general::ConvertBytesToHex(messageId) << std::endl;
}

/* 
    Helper method to encrypt the common symmetric key of the current and the target's public key.
    The whole point: We want to create a secured char\program, so if someone "spies" in the middle of the conversation, then he cannot understand anything,
    because the symmetric key will be encrypted, so only the current client can understand with the symmetric key he generated 
    and the target's client by decrypting with the private key stored locally.
*/
std::vector<uint8_t> ResponseHandler::PrepareEncryptedSymmetricKey(const std::vector<uint8_t>& targetClientId, const std::vector<uint8_t>& publicKeyBytes) {


    // Defensive purpose: first we check the parameters are legal -> Client ID is 16 bytes and public Key is 160 bytes
    if (targetClientId.size() != 16 || publicKeyBytes.size() != 160) {
        std::cout << "Invalid target's client ID or public key size!" << std::endl;
        return {};
    }

    // Check if the given client already have\had communicate with me, so the symmetric key already exists
    for (auto& member : membersList) {
        if (member.id == targetClientId) {

            // There was no communicate yet, so no symmetric key was generated. A new symmetric key will be generated and stored for the next time (saves time)
            if (member.symmetricKey.empty()) {
                unsigned char keyBuffer[16];
                AESWrapper::GenerateKey(keyBuffer, 16);

                member.symmetricKey.assign(keyBuffer, keyBuffer + 16);
            }

            // Encryption of the symmetric key with the target's public key -> will be unlocked (decrypted) with private key
            std::string publicKey(publicKeyBytes.begin(), publicKeyBytes.end());
            RSAPublicWrapper rsa(publicKey);

            std::string symmetricKeyToEncrypt(member.symmetricKey.begin(), member.symmetricKey.end());
            std::string encryptedSymmetricKey = rsa.encrypt(symmetricKeyToEncrypt);

            std::vector<uint8_t> encryptedBytes(encryptedSymmetricKey.begin(), encryptedSymmetricKey.end());
            return encryptedBytes;
        }
    }

    std::cout << "Target client was not found in members list!" << std::endl;
    return {};
}

/* members list is a private field, so we need to make it accessible from other files */
std::vector<ResponseHandler::ClientInfo>& ResponseHandler::getMembersList() {
    return ResponseHandler::membersList;
}

/* 
    For response 2104:
    Find the correct ClientInfo object by its targetId, then access its symmetricKey member.
*/
std::string ResponseHandler::DecryptMessage(std::vector<uint8_t>& message, std::string& targetId, ResponseHandler& serverResponse) {

    // Find the client in the membersList by targetId
    std::vector<uint8_t> targetIdBytes = general::ConvertHexToBytes(targetId);
    std::vector<uint8_t> symmetricKey;

    // Finds the member with the given targetId -> in case wasn't found then symmetricKey will be empty
    for (const auto& member : serverResponse.getMembersList()) {
        if (member.id == targetIdBytes) {
            symmetricKey = member.symmetricKey;
            break;
        }
    }

    if (symmetricKey.empty()) {
        std::cout << "Symmetric key not found for targetId: "<< targetId << std::endl;
        return {};
    }

    // Decrypting the given message with the "found" member's symmetric key.
    AESWrapper aes(symmetricKey.data(), symmetricKey.size());

    std::vector<char> messageBeforeDecryption(message.begin(), message.end());

    std::string decryptedMessage = aes.decrypt(messageBeforeDecryption.data(), messageBeforeDecryption.size());

    return decryptedMessage;
}

/* 
    For response 2104:
    The client get an encrypted symmetric key, decrypt it and stores it inside the ClientInfo of the sender-client with the given ID = targetID
*/
bool ResponseHandler::DecryptAndSaveSymmetricKey(std::string username, std::vector<uint8_t> EncryptedSymmetricKey, std::string targetID, ResponseHandler &serverResponse) {

    std::filesystem::path path = FindWhereEXEFileIs();
    std::filesystem::path usersPath = RemoveFilenameAndSeparator(path, "Users\\" + username + ".info.txt");

    std::string line;
    std::ifstream file(usersPath, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    std::string privateKey = "";

    /* Skip first 2 lines: username + client id */
    std::getline(file, line);
    std::getline(file, line);

    /* Read the rest of the file exactly as raw bytes */
    char ch;
    while (file.get(ch))
    {
        privateKey += ch;
    }

    file.close();

    // Decrypt the symmetric key
    RSAPrivateWrapper rsa(privateKey.c_str(), privateKey.size());

    std::string EncryptedSymmetricKeyText = "";
    for (size_t i = 0; i < EncryptedSymmetricKey.size(); i++) {
        EncryptedSymmetricKeyText += EncryptedSymmetricKey[i];
    }

    std::string decryptedSymmetricKey = rsa.decrypt(EncryptedSymmetricKeyText);

    std::vector<uint8_t> decryptedSymmetricKeyVector;
    for (size_t i = 0; i < decryptedSymmetricKey.size(); i++) {
        decryptedSymmetricKeyVector.push_back(decryptedSymmetricKey[i]);
    }
    
    std::vector<uint8_t> targetIdBytes = general::ConvertHexToBytes(targetID);
    for (auto& member : serverResponse.getMembersList()) {
        if (member.id == targetIdBytes) {
            member.symmetricKey = decryptedSymmetricKeyVector;
            return true;
        }
    }

    return false;
}

/*
    A method to retrieve the username from the members list by its id.
    Mostly used for displays like showing where a message came from (which user sent it), etc...
*/
std::string ResponseHandler::GetANameFromMembersList(ResponseHandler &serverResponse, std::vector<uint8_t> &userId) {
    for (auto& member : serverResponse.getMembersList()) {
        if (userId == member.id) {
            return member.name;
        }
    }
}