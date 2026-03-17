#pragma once
#include <iostream>
#include <algorithm> // For std::all_of() function
#include <string>

namespace general {
    bool isNumber(const std::string_view value);
    std::string trim(const std::string& str,
        const std::string& whitespace = " \t");
    std::string reduce(const std::string& str,
        const std::string& fill = " ",
        const std::string& whitespace = " \t");

    template<const unsigned num, const char separator>

    /* 
        Used mostly in order to display string in hex form.
        https://stackoverflow.com/questions/27952032/how-to-insert-a-character-every-n-characters-in-a-string-in-c
    */
    void separate(std::string& input)
    {
        for (auto it = input.begin(); (num + 1) <= std::distance(it, input.end()); ++it)
        {
            std::advance(it, num);
            it = input.insert(it, separator);
        }
    }

    void ToLittleEndianFromUint16(uint8_t* dest, uint16_t val);
    void ToLittleEndianFromUint32(uint8_t* dest, uint32_t val);
}
