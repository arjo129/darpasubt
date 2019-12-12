#ifndef circular_buffer_h
#define circular_buffer_h

#include <Arduino.h>
#include <util/crc16.h>

namespace CRC
{
    class CRC
    {
        public:
            static uint16_t calc_crc16(uint8_t* data, uint16_t len);
            static uint16_t append_crc16(uint8_t* data, uint16_t len, uint16_t index);
            static bool check_crc16(uint8_t* data, uint16_t len, uint16_t index);
    };
}

#endif