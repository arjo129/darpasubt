#include <Transport.h>

namespace Transport
{
    /**
     * @brief Construct a new Transport:: Transport object.
     * 
     * @param src_addr address of source node.
     */
    Transport::Transport(uint8_t src_addr, RHMesh* net_manager) :
    source_addr(src_addr), 
    net_manager(net_manager)
    {
        this->chunk = 0;
    }

    Transport::~Transport(void) {}

    /**
     * @brief Sends a data Chunk to a given destination address.
     * 
     * @param dest_addr address of destination node.
     * @param chunk Chunk containing the data to be sent.
     */
    void Transport::send(uint8_t dest_addr, Chunk::Chunk* chunk)
    {
        this->chunk = chunk;

        // Break data chunk into segments.
        this->chunk->segment_data(this->source_addr, dest_addr);
        // Send each segment one by one.
        for (uint8_t i = 0; i < this->chunk->get_seg_count(); i++)
        {
            uint8_t buf[SEGMENT_SIZE];
            memset(buf, 0, SEGMENT_SIZE);
            this->chunk->flatten_seg(i, buf);
            net_manager->sendtoWait(buf, sizeof(buf), dest_addr);
            Serial.print(F("sent : 0x"));
            // for (int j = 0; j < SEGMENT_SIZE; j++)
            // {
            //     Serial.print(buf[j], HEX);
            // }
            // Serial.println(F(""));
        }
        // Serial.println(F(""));
    }

    /**
     * @brief Receives a data Chunk from a source address.
     * 
     * @param data bytes array chunk received.
     */
    void Transport::receive(uint8_t* data)
    {
        uint8_t source = 0;
        uint8_t len = RH_MESH_MAX_MESSAGE_LEN;
        if (net_manager->recvfromAck(data, &len, &source))
        {
            Serial.print(F("from "));
            Serial.print(source);
            Serial.print(F(" : "));
            for (int j = 0; j < SEGMENT_SIZE; j++)
            {
                Serial.print((char)data[j]);
            }
            Serial.println(F(""));
            if (CRC::CRC::check_crc16(data, SEGMENT_SIZE, CRC_IDX))
                Serial.println(F("crc correct"));
            else
                Serial.println(F("crc wrong"));
        }
    }
}


