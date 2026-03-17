#include <iostream>
#include <string>
#include <vector>

#include "Request.h"
#include "RequestHandler.h"
#include "GeneralFunctions.h"

RequestHandler::RequestHandler(uint16_t code) {
    this->code = code;
}
std::vector<uint8_t> RequestHandler::HandleRequest700(uint8_t client_id[]) {
    uint16_t code = 700;

    std::cout << "Type the username " << std::endl;

    std::vector<uint8_t> buffer;

    std::string user_text_username;
    std::getline(std::cin, user_text_username);

    std::string user_text_username_padding;
    for (int i = 0; i < 255 - user_text_username.size(); i++) {
        user_text_username_padding += '\0';
    }

    std::cout << "debug - user_txt is: " << user_text_username << std::endl;

    std::string public_key = "FAKE_PUBLIC_KEY";
    std::string public_key_padding;
    for (int i = 0; i < 160 - public_key.size(); i++) {
        public_key_padding += '\0';
    }

    int payload_size = user_text_username.size() + user_text_username_padding.size()
        + public_key.size() + public_key_padding.size();

    uint8_t zero_client_id[16] = { 0 };
    RequestHeader header(zero_client_id, RequestHandler::VERSION, this->code, payload_size);

    buffer.resize(sizeof(header) + user_text_username.size()
        + user_text_username_padding.size()
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

    std::memcpy(buffer.data() + curr_size, user_text_username.data(), user_text_username.size());
    curr_size += user_text_username.size();

    std::memcpy(buffer.data() + curr_size, user_text_username_padding.data(), user_text_username_padding.size());
    curr_size += user_text_username_padding.size();

    std::memcpy(buffer.data() + curr_size, public_key.data(), public_key.size());
    curr_size += public_key.size();

    std::memcpy(buffer.data() + curr_size, public_key_padding.data(), public_key_padding.size());

    return buffer;
}

