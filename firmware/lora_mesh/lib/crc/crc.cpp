#include <crc.h>

namespace CRC
{
    /**
     * @brief Calculates a CRC checksum given an array of bytes.
     * 
     * @param data array of bytes to calculate the CRC checksum.
     * @param len length of the array (in bytes).
     * @return uint16_t CRC value.
     */
    uint16_t CRC::calc_crc16(uint8_t* data, uint16_t len)
    {
        FastCRC16 crc16;
        return crc16.ccitt(data, len);
    }

    /**
     * @brief Appends a 16 bits CRC checksum to the given data array.
     * 
     * @param data array of bytes to append the checksum.
     * @param len length of the array (in bytes).
     * @param index array index to append the first CRC byte, requires 2 bytes of space.
     * @return uint16_t the 16 bits CRC calculated.
     */
    uint16_t CRC::append_crc16(uint8_t* data, uint16_t len, uint16_t index)
    {
        uint16_t crc = calc_crc16(data, len);

        /*  Append the calculated CRC.
            The CRC higher order bits corresponds to lower index value.
        */
        data[index] = (uint8_t)((crc & 0xFF00) >> 8);
        data[index+1] = (uint8_t)(crc & 0x00FF);

        return crc;
    }

    /**
     * @brief Check for CRC errors.
     * 
     * @param data array of bytes to check.
     * @param len length of the array (in bytes).
     * @param index array index of the first CRC byte.
     * @return true if CRC is consistent with data.
     * @return false if CRC is inconsistent with data.
     */
    bool CRC::check_crc16(uint8_t* data, uint16_t len, uint16_t index)
    {
        bool correct;

        // The CRC higher order bits corresponds to lower index value.
        uint8_t crc_high = data[index];
        uint8_t crc_low = data[index+1];

        // Check the higher and lower order 8 bits separately.
        data[index] = 0; // Clear CRC bytes to zeroes first.
        data[index+1] = 0;
        uint16_t chk = calc_crc16(data, len);
        if (crc_high == ((chk & 0xFF00) >> 8) && crc_low == (chk & 0x00FF))
            correct = true;
        else
            correct = false;

        data[index] = crc_high;
        data[index] = crc_low;
        return correct;
    }
}