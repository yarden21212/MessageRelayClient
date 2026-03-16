#include "Request.h"

RequestHeader::RequestHeader(const uint8_t client_id[16], uint8_t version, uint16_t code, uint32_t payload_size) {
	std::memcpy(this->client_id, client_id, 16);
	this->version = version;
	this->code = code;
	this->payload_size = payload_size;
};

//int main() {
//	std::cout << "size of RequestHeader is: " << sizeof(RequestHeader) << std::endl;
//
//
//	std::system("pause");
//	return 0;
//}
//
