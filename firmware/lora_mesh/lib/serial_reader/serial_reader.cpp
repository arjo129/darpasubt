#include <serial_reader.h>

namespace serial_reader
{
    circular_buffer read_queue(QUEUE_SIZE);
    uint8_t buffer[SERIAL_READ_SIZE] = {0};
    uint16_t current_len = 0;
    uint16_t recv_len = 0;
    uint8_t recv_len_check_count = 0;

    bool receiving_len = false;
    bool receiving_body = false;
    
    /**
     * @brief Reads from Serial large batches of data. Each batch is added to a queue as a Chunk
     *        object which can be retrieved later. 
     *        NOTE:
     *        If incoming data comes in faster than the Chunks in the queue can be processed, 
     *        incoming data will be dropped.
     * 
     *        Each batch is indicated by the start marker '<' and end marker '>'.
     *        The batch must also include the length in bytes of the batch and destination address.
     *        Anatomy:
     *        <[length]...payload...[dest]>
     *        
     *        * The length is 16 bits unsigned integer and destination is 8 bits unsigned integer.
     * 
     * TODO: Standardise the anatomy of each batch of data.
     *       
     */
    void read(void)
    {
        while (Serial.available() && !read_queue.full())
        {
            byte bite = Serial.read();

            if (bite == BEGIN_CHAR)
            {
                // A new chunk of data is detected, prepare to read until the end marker.
                clear();
                receiving_len = true;
            }
            else if (receiving_len)
            {
                if (recv_len_check_count == 0) // The higher 8 bits of the length.
                {
                    recv_len |= (uint16_t)bite << 8;
                }
                if (recv_len_check_count == 1) // The lower 8 bits of the length.
                {
                    recv_len |= bite;
                }
                
                recv_len_check_count++;
                if (recv_len_check_count >= RECV_LEN_SIZE)
                {
                    // We have finish reading the length, transit to reading the payload
                    receiving_body = true;
                    receiving_len = false;
                }
            }
            else if (receiving_body)
            {
                if (bite == END_CHAR)
                {
                    receiving_body = false;

                    // Save the buffer as a Chunk into the queue.
                    uint8_t dest = buffer[current_len - 1];
                    Chunk::Chunk new_chunk(buffer, current_len-1, dest);
                    read_queue.put(new_chunk);
                }
                else
                {
                    if (current_len >= recv_len)
                    {
                        continue; // Ignore bytes beyond the specified length.
                    }
                    else
                    {
                        buffer[current_len++] = (uint8_t)bite;
                    }
                }
            }
        }
    }

    /**
     * @brief Clear and reset all variables. Must be called after every termination!
     * 
     */
    void clear(void)
    {
        memset(buffer, 0, SERIAL_READ_SIZE);
        current_len = 0;
        recv_len = 0;
        recv_len_check_count = 0;
    }
}