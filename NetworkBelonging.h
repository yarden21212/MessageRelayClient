#pragma once
#include <iostream>
#include <memory>
#include <string> // For stoi() string to integer
#include <string_view>
#include <vector>
#include <algorithm>

//std::string returnNetworkPart(std::string_view networkInfo, const std::string part);
//bool checkSocketForm(const std::string ipOrPort, const std::string part);
//bool isNumber(const std::string_view value);
//std::string extractPort(std::string networkInfo);
std::string networkPart(char address[], std::string_view part);