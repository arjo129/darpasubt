#include <serial_reader.h>

namespace serial_reader
{
    uint8_t buffer[SERIAL_READ_SIZE] = {0};
    uint16_t current_len = 0;
    bool terminate = false;

    /**
     * @brief Reads from Serial until the terminating character is read or the maximum size for a
     *        Chunk is reached. In those cases, the terminate flag is set and no more bytes will
     *        be further read.
     * 
     */
    void read(void)
    {
        while (Serial.available())
        {
            if (current_len >= SERIAL_READ_SIZE)
            {
                terminate = true;
                break;
            }

            byte bite = Serial.read();
            if ((char)bite == TERMINATE_CHAR)
            {
                if (current_len > 0) 
                    terminate = true;

                break;
            }
            else
            {
                buffer[current_len] = bite;
                current_len++;
            }
        }

        if (current_len >= SERIAL_READ_SIZE)
            terminate = true;
    }

    /**
     * @brief Clear and reset all variables. Must be called after every termination!
     * 
     */
    void clear(void)
    {
        memset(buffer, 0, SERIAL_READ_SIZE);
        current_len = 0;
        terminate = false;
    }
}