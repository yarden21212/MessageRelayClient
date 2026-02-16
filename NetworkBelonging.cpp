#include "NetworkBelonging.h"

std::string networkPart(char address[], std::string_view part){
    
    if (part == "ip" || part == "port") {
        int octet1, octet2, octet3, octet4, port;
        int n = sscanf_s(address, "%d.%d.%d.%d:%d", &octet1, &octet2, &octet3, &octet4, &port);

        if (n == 5) {
            if (part == "ip")
            {
                std::string ip = std::to_string(octet1) + "." + std::to_string(octet2) + "." +
                    std::to_string(octet3) + "." + std::to_string(octet4);
                return ip;
            }
                
            else {
                return std::to_string(port);
            }
        }
        else {
            throw std::invalid_argument("Illegal socket address format. Expected format: Octet1.Octet2.Octet3.Octet4:PORT");
        }
    }
    else {
        throw std::invalid_argument("Illegal input. type \"ip\" or \"port\" please!\n");
    }

}

//std::string returnNetworkPart(std::string_view networkInfo, std::string part) {
//
//    std::string ip;
//    std::string port;
//
//    std::string::size_type pos = networkInfo.find(':');
//
//    ip = networkInfo.substr(0, pos);
//    port = networkInfo.substr(pos + 1);
//
//    if (part == "ip") {
//        bool legal = checkSocketForm(ip, "ip");
//        if (legal)
//            return ip;
//    }
//    else if (part == "port") {
//        bool legal = checkSocketForm(port, "port");
//        if (legal)
//            return extractPort(port);
//    }
//    else
//        std::cout << "Illegal request, need to ask for ip or port" << std::endl;
//}
//
//bool isNumber(const std::string_view value) {
//    if (!value.empty() && std::all_of(value.begin(), value.end(), [](char c) {
//        return ::isdigit(c);
//        }))
//    {
//        return true;
//    }
//
//    return false;
//}
//
//
//bool checkSocketForm(const std::string ipOrPort, const std::string part) {
//    bool number{ false };
//    int portNumber;
//
//    if (part == "port") {
//
//        //The condition checks if the port contains only numeric values.
//        if (isNumber(ipOrPort)) {
//            portNumber = std::stoi(ipOrPort);
//
//            if ((0 <= portNumber && portNumber <= 65535))
//            {
//                return true;
//            }
//
//        }
//
//        //throw std::invalid_argument("Port has illegal form.");
//        return false;
//    }
//    else if (part == "ip") {
//        bool legal = false;
//        bool end = false;
//
//        std::string::size_type currSpot;
//        std::string::size_type currDot;
//        std::string currString = ipOrPort;
//        for (int i = 0; i < 3; i++) {
//
//            (i == 3) ? end = true : end = false; //Reached the last segment, if it's legal then we can finish
//
//            currSpot = 0;
//            currDot = currString.find('.');
//
//            std::string segment = currString.substr(currSpot, currDot);
//
//            currString = currString.substr(currDot + 1);
//
//            currSpot = currDot + 1;
//
//
//            std::cout << "Segment is: " << segment << std::endl;
//            if (isNumber(segment)) {
//                int value = stoi(segment);
//
//                (0 <= value && value <= 255) ? legal = true : legal = false;
//
//                if (!legal) {
//
//                    std::cout << "Value is: " << value << std::endl;
//                    return false;
//                }
//
//            }
//            else {
//                return false;
//            }
//
//        }
//
//        if (end) {
//            return true;
//        }
//    }
//
//    return true;
//
//}
//
//
//std::string extractPort(std::string networkInfo) {
//	std::string::size_type socketAddressSeperation = networkInfo.find(':');
//	std::string portPart = networkInfo.substr(socketAddressSeperation + 1, networkInfo.length());
//
//	return portPart;
//}

//Implement later

//#include <print>
//#include <regex>
//#include <string>
//#include <array>
//#include <charconv>
//
//int main() {
//    std::array test_cases = {
//        "254.254.254.127",
//        "2a4.254.254.127",
//        "254.254.254.127:1234",
//        "254.25.254.12",
//    };
//
//    for (auto ip : test_cases) {
//        int a, b, c, d, port;
//        int n = std::sscanf(ip, "%d.%d.%d.%d:%d",
//            &a, &b, &c, &d, &port);
//        if (n >= 4) {
//            std::println("\n\nIP String: {}", ip);
//            std::println("Octet 1: {}", a);
//            std::println("Octet 2: {}", b);
//            std::println("Octet 3: {}", c);
//            std::println("Octet 4: {}", d);
//            if (n == 5) // port was included
//                std::println("Port: {}", port);
//        }
//    }
//}