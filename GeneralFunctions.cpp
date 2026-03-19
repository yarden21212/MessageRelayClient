#include "GeneralFunctions.h"
#include "FileFunctionality.h"

#include "CustomExceptions.h"


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


}