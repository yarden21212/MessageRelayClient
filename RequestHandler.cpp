#include <iostream>
#include <string>
#include <vector>

#include "Request.h"
#include "RequestHandler.h"
#include "GeneralFunctions.h"

#include "CustomExceptions.h"
/* For encryption -> public, private keys */
#include <rsa.h>
#include "RSAWrapper.h"

RequestHandler::RequestHandler(uint16_t code) {
    this->code = code;
}
std::vector<uint8_t> RequestHandler::HandleRequest700(std::string &userTextUsername, uint8_t client_id[]) {
    RSAPrivateWrapper rsa;
    auto publicKey = rsa.getPublicKey();

    if (!general::ValidatePublicKey(publicKey, 160))
    {
        throw IllegalSizeException("[ERROR]: Public key has to be 160 characters. Try again!");   
    }

    uint16_t code = 700;

    std::vector<uint8_t> buffer;

    std::string userTextUsernamePadding;
    for (int i = 0; i < 255 - userTextUsername.size(); i++) {
        userTextUsernamePadding += '\0';
    }

    std::cout << "\n" << userTextUsername << std::endl;
    std::cout << "debug - user_txt is: " << userTextUsername << std::endl;

    std::string public_key = "FAKE_PUBLIC_KEY";
    std::string public_key_padding;
    for (int i = 0; i < 160 - public_key.size(); i++) {
        public_key_padding += '\0';
    }

    int payload_size = userTextUsername.size() + userTextUsernamePadding.size()
        + public_key.size() + public_key_padding.size();

    uint8_t zero_client_id[16] = { 0 };
    RequestHeader header(zero_client_id, RequestHandler::VERSION, this->code, payload_size);

    buffer.resize(sizeof(header) + userTextUsername.size()
        + userTextUsernamePadding.size()
        + public_key.size()
        + public_key_padding.size());

    int curr_size = 0;

    /* Create new version of the header for supporting every platform -> Will support little-endian*/

    // version [1 byte] -> doesn't really need
    uint8_t version_to_little_endian = header.version;
    // code [2 bytes]
    uint8_t code_to_little_endian[2];
    general::ToLittleEndianFromUint16(code_to_little_endian, header.code);

    // payload_size [4 bytes]
    uint8_t payload_size_to_little_endian[4];
    general::ToLittleEndianFromUint32(payload_size_to_little_endian, header.payload_size);

    // Copying the data (including little-endian) to the buffer: [header][payload] -> [header][user name][public key]
    std::memcpy(buffer.data(), header.client_id, sizeof(header.client_id));
    curr_size += sizeof(header.client_id);

    std::memcpy(buffer.data() + curr_size, &version_to_little_endian, sizeof(version_to_little_endian));
    curr_size += sizeof(version_to_little_endian);

    std::memcpy(buffer.data() + curr_size, code_to_little_endian, sizeof(code_to_little_endian));
    curr_size += sizeof(code_to_little_endian);

    std::memcpy(buffer.data() + curr_size, payload_size_to_little_endian, sizeof(payload_size_to_little_endian));
    curr_size += sizeof(payload_size_to_little_endian);

    std::memcpy(buffer.data() + curr_size, userTextUsername.data(), userTextUsername.size());
    curr_size += userTextUsername.size();

    std::memcpy(buffer.data() + curr_size, userTextUsernamePadding.data(), userTextUsernamePadding.size());
    curr_size += userTextUsernamePadding.size();

    std::memcpy(buffer.data() + curr_size, public_key.data(), public_key.size());
    curr_size += public_key.size();

    std::memcpy(buffer.data() + curr_size, public_key_padding.data(), public_key_padding.size());

    return buffer;
}

