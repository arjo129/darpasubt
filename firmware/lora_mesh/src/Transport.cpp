#include <Transport.h>

namespace Transport
{
    /**
     * @brief Construct a new Transport:: Transport object.
     * 
     * @param src_addr address of source node.
     */
    Transport::Transport(uint8_t src_addr, RHMesh* net_manager) :
        src_addr(src_addr),
        net_manager(net_manager),
        queue(MAX_BUFFER_SIZE)
    {
    }

    /**
     * @brief Destroy the Transport:: Transport object.
     * 
     */
    Transport::~Transport(void) 
    {}

    /**
     * @brief Push a chunk into the processing queue.
     * 
     * @param chunk 
     */
    void Transport::queue_chunk(Chunk::Chunk& chunk)
    {
        queue.put(chunk);
    }

    /**
     * @brief Sends out one Chunk if there is any in the sending queue.
     * 
     */
    void Transport::process_queue(void)
    {
        if (!queue.empty())
        {
            Chunk::Chunk chunk = queue.get();
            send(chunk);
        }
    }

    /**
     * @brief Sends a data Chunk to a given destination address.
     * 
     * @param chunk Chunk containing the data to be sent.
     */
    void Transport::send(Chunk::Chunk chunk)
    {
        // Break data chunk into segments.
        chunk.segment_data(this->src_addr);
        // Send each segment one by one.
        for (uint8_t i = 0; i < chunk.get_seg_count(); i++)
        {
            uint8_t buf[SEGMENT_SIZE] = {0};
            chunk.flatten_seg(i, buf);
            net_manager->sendtoWait(buf, sizeof(buf), chunk.get_dest());
            Serial.print(F("sent : 0x"));
            for (int j = 0; j < SEGMENT_SIZE; j++)
            {
                Serial.print(buf[j], HEX);
                Serial.print(F(" "));
            }
            Serial.println(F(""));
        }
        Serial.println(F(""));
    }

    /**
     * @brief Receives a data Chunk from a source address.
     * 
     * @return Chunk::Chunk received data Chunk.
     */
    Chunk::Chunk Transport::receive(void)
    {
        uint8_t source = 0;
        uint8_t buf[SEGMENT_SIZE] = {0};
        uint8_t len = sizeof(buf);
        if (net_manager->recvfromAck(buf, &len, &source))
        {
            Chunk::Segment segment = Chunk::Chunk::expand_seg(buf);

            if (Chunk::Chunk::check_syn(segment.flags_offset))
            {
                bool wrong_chunk = false;
                Chunk::Chunk recv_chunk;
                recv_chunk.assemble_seg(segment);
                memset(buf, 0, len);

                while (net_manager->recvfromAck(buf, &len, &source))
                {
                    segment = Chunk::Chunk::expand_seg(buf);
                    bool res = recv_chunk.assemble_seg(segment);
                    if (!res)
                    {
                        wrong_chunk = true;
                        break;
                    }
                    if (!Chunk::Chunk::check_mf(segment.flags_offset))
                    {
                        break;
                    }
                }

                if (wrong_chunk)
                {
                    Chunk::Chunk empty;
                    return empty;
                }
                else
                {
                    recv_chunk.reassemble();
                    return recv_chunk;
                }
            }
            
            // Serial.print(F("from "));
            // Serial.print(source);
            // Serial.print(F(" : "));
            // for (int j = 0; j < SEGMENT_SIZE; j++)
            // {
            //     Serial.print(data[j], HEX);
            // }
            // Serial.println(F(""));
            // if (CRC::CRC::check_crc16(data, SEGMENT_SIZE, CRC_IDX))
            //     Serial.println(F("crc correct"));
            // else
            //     Serial.println(F("crc wrong"));
        }
    }
}


