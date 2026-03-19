#pragma once

#include <iostream>
#include <cstdint>
#include <vector>



class ResponseHandler {
private:
	uint16_t code;

public:
	ResponseHandler(uint16_t code);
	void HandleResponse9000(std::vector<uint8_t>& payload);
	std::string HandleResponse2100(std::vector<uint8_t>& payload);
};