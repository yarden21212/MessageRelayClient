#include "NetworkBelonging.h"

/*
    Checks if the given socket address is legal.
    Legal ip address has 4 parts called octets separated with 3 dots.
    The socket includes ':' and then a number which is the port.
    Form: w.x.y.z:port

    If the form is illegal, an error will be thrown
*/
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
