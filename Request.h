#pragma once
#pragma pack(push, 1)

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

struct RequestHeader {
    uint8_t client_id[16];
    uint8_t version;
    uint16_t code;
    uint32_t payload_size;

    RequestHeader(const uint8_t client_id[16], uint8_t version, uint16_t code, uint32_t payload_size);
};

#pragma pack(pop)

struct ResponseHeader {
    uint8_t version;
    uint16_t code;
    uint32_t payload_size;

    void BuildResponseHeader(uint8_t response[]);
};
