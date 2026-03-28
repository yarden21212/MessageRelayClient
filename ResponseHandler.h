#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>  // Includes ostringstream
#include <iomanip> // Includes setfill and setw
#include "GeneralFunctions.h"
#include "CustomExceptions.h"
#include "AESWrapper.h"
#include "RSAWrapper.h"
#include "FileFunctionality.h"


class ResponseHandler {
private:
	uint16_t code;
	struct ClientInfo { // Stores one client from response 2101
		std::string name; // client's username
		std::vector<uint8_t> id; // client ID
		std::vector<uint8_t> symmetricKey; // Client symmetric key
	};

	std::vector<ClientInfo> membersList; // Saved members list for later use in request 702

public:
	ResponseHandler(uint16_t code);
	/* Methods to produce output for the different type of responses */
	void HandleResponse9000(std::vector<uint8_t>& payload);
	std::string HandleResponse2100(std::vector<uint8_t>& payload);
	void HandleResponse2101(const std::vector<uint8_t>& payload, const std::string& username);
	std::vector<uint8_t> HandleResponse2102(const std::vector<uint8_t>& payload);
	void HandleResponse2103(const std::vector<uint8_t>& payload);

	//Different helper methods:
	std::string GetANameFromMembersList(ResponseHandler& serverResponse, std::vector<uint8_t>& fromId);
	std::vector<uint8_t> PrepareEncryptedSymmetricKey(const std::vector<uint8_t>& targetClientId, const std::vector<uint8_t>& publicKeyBytes);
	std::string DecryptMessage(std::vector<uint8_t>& message, std::string& targetId, ResponseHandler& serverResponse);
	bool DecryptAndSaveSymmetricKey(std::string username, std::vector<uint8_t> EncryptedSymmetricKey, std::string targetID, ResponseHandler& serverResponse);
	std::vector<uint8_t> GetClientIdByName(const std::string& clientName);
	std::vector<ClientInfo>& getMembersList();
};                            
