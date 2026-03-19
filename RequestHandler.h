#pragma once

#include <iostream>
#include <string>
#include "Request.h" // Do I need this? need to check
#include <vector>

class RequestHandler {
private:
	uint16_t code;
public:
	static constexpr  uint8_t VERSION = 2;
public:
	RequestHandler(uint16_t code);
	std::vector<uint8_t> HandleRequest700(std::string& userName, uint8_t client_id[]);

};
