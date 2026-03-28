#pragma once

#include <cstdlib>
#include <cstring> // gives you access to C-style string and memory functions
#include <iostream>
#include <boost/asio.hpp>
#include <format>
#include "NetworkBelonging.h"
#include "ClientMenu.h"
#include "GeneralFunctions.h"
#include "Request.h"
#include <vector>

#include "RequestHandler.h"
#include "ResponseHandler.h"
#include "FileFunctionality.h"

#include "CustomExceptions.h"

#include <rsa.h>
#include "RSAWrapper.h"



void clear(char* buffer, int length);
int TCPConnector(std::string_view ip, std::string_view port);