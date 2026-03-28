#include "Request.h"

RequestHeader::RequestHeader(const uint8_t client_id[16], uint8_t version, uint16_t code, uint32_t payload_size) {
	std::memcpy(this->client_id, client_id, 16);
	this->version = version;
	this->code = code;
	this->payload_size = payload_size;
};
void ResponseHeader::BuildResponseHeader(uint8_t *response) {


	// << 8 -> it's like multiplying by 2^8 = 256
	this->version = response[0];
	this->code = (response[2] << 8) + response[1];
	this->payload_size = (response[6] << (8 * 3)) + (response[5] << (8 * 2)) + (response[4] << 8) + response[3];
}



//int main() {
//	uint8_t value = 5;
//	std::cout << "value = " << unsigned(value) << std::endl;
//
//	value = ToLittleEndian(value);
//
//	std::cout << unsigned(value) << std::endl;
//
//	std::system("pause");
//	return 0;
//}
//
