#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <set>
#include "Request.h"
#include "GeneralFunctions.h"
#include "FileFunctionality.h"
#include "CustomExceptions.h"
#include "ResponseHandler.h"

/* For encryption -> public, private keys */
#include <rsa.h>
#include "RSAWrapper.h"
#include "AESWrapper.h"






class RequestHandler {
private:
	uint16_t code;
public:
	static constexpr  uint8_t VERSION = 2;
public:
	RequestHandler(uint16_t code);
	std::vector<uint8_t> HandleRequest700(std::string& userName, const std::string& publicKey);
	std::vector<uint8_t> HandleRequest701(std::string& username);
	std::vector<uint8_t> HandleRequest702(std::string& username, const std::string& targetIdHex);
	std::vector<uint8_t> HandleRequest703(std::string& username, const std::vector<uint8_t> targetIdBytes, const uint8_t messageType, ResponseHandler &serverResponse);
	std::vector<uint8_t> HandleRequest703Type2(std::string& username, const std::vector<uint8_t>& targetIdBytes, const std::vector<uint8_t>& encryptedSymmetricKey);
	std::vector<uint8_t> HandleRequest704(std::string userTextUsername);
};
