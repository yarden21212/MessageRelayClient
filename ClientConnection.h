#pragma once

#include <cstdlib>
#include <cstring> // gives you access to C-style string and memory functions
#include <iostream>
#include <boost/asio.hpp>
#include <format>


void clear(char* buffer, int length);
int TCPConnector(std::string_view ip, std::string_view port);