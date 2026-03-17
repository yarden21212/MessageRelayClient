#include "ResponseHandler.h"
#include <iostream>
#include <sstream>  // Includes ostringstream
#include <iomanip> // Includes setfill and setw
#include "GeneralFunctions.h"

ResponseHandler::ResponseHandler(uint16_t code) {
	this->code = code;
}

void ResponseHandler::HandleResponse9000(std::vector<uint8_t> &payload) {
	payload.resize(0);
	std::cout << "Response:\nResponse code:" << ResponseHandler::code << std::endl;
	std::cout << "User already exists or invalid input sent!:\n" << std::endl;
}

void ResponseHandler::HandleResponse2100(std::vector<uint8_t>& payload) {

    std::cout << "[HandleResponse2100 DEBUG]:" << payload.data() << std::endl;

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

    general::separate<2, ' '>(s);
    std::cout << "Server: Client ID:" << s << std::endl;

    std::cout << std::endl;
}