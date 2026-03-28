#include "GeneralFunctions.h"
#include "ResponseHandler.h"

#include <filesystem>


namespace general {
    /* 
        Checks if a given string is a number.
        Contains only digits?    
    */
    bool isNumber(const std::string_view value) {
        if (!value.empty() && std::all_of(value.begin(), value.end(), [](char c) {
            return ::isdigit(c);
            }))
        {
            return true;
        }

        return false;
    }
    /*
        Erases extra white-spaces before and after words
    */
    std::string trim(const std::string& str,
        const std::string& whitespace)
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos)
            return ""; // no content

        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;

        return str.substr(strBegin, strRange);
    }

    /*
        Improves trim() by deleting also unnecessary white-spaces between words
        read more here: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
    */
    std::string reduce(const std::string& str, const std::string& fill, const std::string& whitespace)
    {
        // trim first
        auto result = trim(str, whitespace);

        // replace sub ranges
        auto beginSpace = result.find_first_of(whitespace);
        while (beginSpace != std::string::npos)
        {
            const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
            const auto range = endSpace - beginSpace;

            result.replace(beginSpace, range, fill);

            const auto newStart = beginSpace + fill.length();
            beginSpace = result.find_first_of(whitespace, newStart);
        }

        return result;
    }

    // Creates the "little-endian" mechanism in order to support all platforms. Moves the left 2 btyes to the right and the right 2 bytes to the left
    void ToLittleEndianFromUint16(uint8_t* dest, uint16_t val) {
        dest[0] = static_cast<uint8_t>(val); // -> Takes the lowest 8 bits and drops the heighest 8 bits (by casting) casting means slicing the 8 heighest bits
        dest[1] = static_cast<uint8_t>(val >> 8); // -> Moves the 8 heighest bits to the right and then taking the new "lowest 8 bits" (by casting)
    }

    // Same logic as the above method but with 8 bytes
    void ToLittleEndianFromUint32(uint8_t* dest, uint32_t val) {
        dest[0] = static_cast<uint8_t>(val);
        dest[1] = static_cast<uint8_t>(val >> 8);
        dest[2] = static_cast<uint8_t>(val >> 16);
        dest[3] = static_cast<uint8_t>(val >> 24);
    }

    bool ValidateUsername(const std::string &username, const int &maxLength) {
        const int usernameLength = username.length();
        if (usernameLength > maxLength || usernameLength < 1)
            return false;

        return true;
    }

    bool ValidatePublicKey(const std::string& publicKey, const int& length) {
        if (publicKey.length() != length)
            return false;

        return true;
    }
    /*
        Takes a bytes representation and converts it into hex representation.
        Will be used when I want to show the client a representation which is human-readable.
    */
    std::string ConvertBytesToHex(const std::vector<uint8_t>& payload) {

        std::ostringstream x;
        x << std::hex << std::setfill('0');
        auto const& w{ std::setw(2) };
        for (auto b : payload)
            x << w << int(b);
        std::string uniqueIdentifierHex{ x.str() };

        return uniqueIdentifierHex;
    }

    /*
        Convert hex string to byte array (using sscanf)
        More explanation here: https://www.codestudy.net/blog/converting-a-hex-string-to-a-byte-array/
    */
    std::vector<uint8_t> ConvertHexToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        bytes.reserve((hex.size() + 1) / 2); // Reserve space for efficiency

        for (size_t i = 0; i < hex.size(); i += 2) {
            uint8_t byte;
            // Extract 1 or 2 characters (handle odd lengths)
            std::string chunk = hex.substr(i, 2); // substr(i, 2) returns 1 char if i is last

            // Use sscanf to parse hex chunk into byte
            if (sscanf_s(chunk.c_str(), "%hhx", &byte) != 1) {
                throw std::invalid_argument("Invalid hex string: failed to parse chunk '" + chunk + "'");
            }

            bytes.push_back(byte);
        }

        return bytes;
    }

    /*
       Access the "Users" folder inside the folder where "client.exe" is stored.
       We first check if "Users" even exists -> then access the specific username.info we were given as parameter
       -> and then takes its second-row\second-line which represents the client-id
   */
    std::string getUserId(std::string& username) {
        bool usersFolderExists = checkIfFileOrFolderExistsInExeFolder("Users");
        if (!usersFolderExists) {
            std::cout << "User is not registered!" << std::endl;
            return "";
        }
        else {
            std::filesystem::path path = RemoveFilenameAndSeparator(FindWhereEXEFileIs(), "Users");
            path += "\\" + username + ".info.txt";

            std::ifstream myFile(path);
            if (myFile.is_open()) {
                std::string line;
                int lineNumber = 1;
                while (std::getline(myFile, line))
                {
                    if (lineNumber == 2) {

                        //std::cout << "Client ID: " << line << std::endl;
                        return line;
                    }

                    lineNumber++;
                }

                std::cout << "Didn't find the line, something is wrong with the file it self." << std::endl;
                return "";
            }
            else {
                std::cout << "Unable to open file";
                return "";
            }
        }
    }

    std::vector<uint8_t> TargetNameExists(ResponseHandler& serverResponse) {
        std::string targetName;
        std::cout << "Type target client name: ";
        std::getline(std::cin, targetName);

        std::vector<uint8_t> targetIdBytes = serverResponse.GetClientIdByName(targetName);

        if (targetIdBytes.empty()) {
            std::cout << "Client name not found in the members list." << std::endl;
            return {};
        }



        return targetIdBytes;
    }
}