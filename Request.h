#pragma once
#pragma pack(push, 1)

#include <iostream>
#include <cstdint>
#include <cstring>

struct RequestHeader {
    uint8_t client_id[16];
    uint8_t version;
    uint16_t code;
    uint32_t payload_size;

    RequestHeader(const uint8_t client_id[16], uint8_t version, uint16_t code, uint32_t payload_size);
};

#pragma pack(pop)