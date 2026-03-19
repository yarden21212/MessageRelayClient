#include "ResponseHandler.h"
#include <iostream>
#include <sstream>  // Includes ostringstream
#include <iomanip> // Includes setfill and setw
#include "GeneralFunctions.h"
#include "CustomExceptions.h"

ResponseHandler::ResponseHandler(uint16_t code) {
	this->code = code;
}

void ResponseHandler::HandleResponse9000(std::vector<uint8_t> &payload) {
	payload.resize(0);
	std::cout << "Response:\nResponse code:" << ResponseHandler::code << std::endl;
	std::cout << "User already exists or invalid input sent!:\n" << std::endl;
}

std::string ResponseHandler::HandleResponse2100(std::vector<uint8_t>& payload) {

    std::ostringstream x;
    x << std::hex << std::setfill('0');
    auto const& w{ std::setw(2) };
    for (auto b : payload)
        x << w << int(b);
    std::string uniqueIdentifierHex{ x.str() };

    std::cout << "Response:\nResponse code:" << code << std::endl;
    std::cout << "Server: Client ID:" << uniqueIdentifierHex << std::endl;
    std::cout << std::endl;

    return uniqueIdentifierHex;



/*    std::cout << "[HandleResponse2100 DEBUG]: ";
    for (auto b : payload) std::cout << int(b) << ' ';
    std::cout << std::endl;

    std::cout << "Response:\nResponse code:" << code << std::endl;

    std::string uniqueIdentifier(payload.begin(), payload.end());
    std::cout << "unique identifier: ";
    for (unsigned char c : uniqueIdentifier) std::cout << int(c) << ' ';
    std::cout << std::endl;

    std::ostringstream x;
    x << std::hex << std::setfill('0');
    auto const& w{ std::setw(2) };
    for (auto b : payload)
        x << w << int(b);
    std::string uniqueIdentifierHex{ x.str() };

    general::separate<0, ' '>(uniqueIdentifierHex);
    std::cout << "Server: Client ID:" << uniqueIdentifierHex << std::endl;

    std::cout << std::endl;

    return uniqueIdentifierHex*/;
}