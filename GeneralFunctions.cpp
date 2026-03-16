#include "GeneralFunctions.h"

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



}