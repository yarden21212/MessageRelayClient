#include "GeneralFunctions.h"

bool isNumber(const std::string_view value) {
    if (!value.empty() && std::all_of(value.begin(), value.end(), [](char c) {
        return ::isdigit(c);
        }))
    {
        return true;
    }

    return false;
}