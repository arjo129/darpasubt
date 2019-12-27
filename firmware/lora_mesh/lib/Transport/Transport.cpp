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
        send_queue(MAX_BUFFER_SIZE)
        // recv_queue(MAX_BUFFER_SIZE)
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
     * @param chunk Chunk to be queued.
     */
    void Transport::queue_chunk(Chunk::Chunk chunk)
    {
        send_queue.put(chunk);
    }

    /**
     * @brief Sends out one Chunk if there is any in the sending queue.
     * 
     */
    void Transport::process_send_queue(void)
    {
        if (!send_queue.empty())
        {
            Chunk::Chunk chunk = send_queue.get();
            if (!send(chunk))
            {
                chunk.increment_attempts();
                if (chunk.get_attempts() <= MAX_ATTEMPTS)
                    send_queue.put(chunk);
            }
        }
    }

    /**
     * @brief Sends a data Chunk to a given destination address.
     * 
     * @param chunk Chunk containing the data to be sent.
     * @return true if every Segment was transmitted successfully.
     * @return false if at least one Segment failed to transmit.
     */
    bool Transport::send(Chunk::Chunk& chunk)
    {
        // Break data chunk into segments.
        if (chunk.get_attempts() == 0)
            chunk.segment_data(this->src_addr);
        // Send each segment one by one.
        for (uint8_t i = 0; i < chunk.get_seg_count(); i++)
        {
            uint8_t buf[SEGMENT_SIZE] = {0};
            chunk.get_flat_segment(i, buf);

            if (net_manager->sendtoWait(buf, sizeof(buf), chunk.get_dest()) != RH_ROUTER_ERROR_NONE)
                return false;
            
            Serial.print(F("sent : 0x"));
            for (int j = 0; j < SEGMENT_SIZE; j++)
            {
                Serial.print(buf[j], HEX);
                Serial.print(F(" "));
            }
            Serial.println(F(""));
        }
        Serial.println(F(""));

        return true;
    }

    /**
     * @brief Process and take actions according to the context of the Segment.
     *        A Segment can be one of the following three cases:
     *        (1):
     *        The Segment is the first and only Segment of a Chunk.
     *        In this case, the Segment's offset == zero and MF bit is set to '0'.
     *        The action to take is to create a new Chunk consisting of the payload.
     *
     *        (2):
     *        The Segment belongs to a Chunk consisting of more than one Segments but it is not
     *        the last Segment.
     *        In this case, the Segments's MF bit will be set to '1' and offset >= 0.
     *        The action to take is to first check if a Chunk for this Segment exists in the
     *        recv_queue. If it does not exist, create new Chunk and insert this Segment.
     *        Otherwise, insert this Segment.
     *
     *        (3):
     *        The Segment is the last Segment of a Chunk.
     *        In this case, the Segment's offset > 0 and MF bit is set to '0'.
     *        The action to take is to first check if a Chunk for this Segment exists in the
     *        recv_queue. If it does not exist, create new Chunk and insert this Segment.
     *        Otherwise, insert this Segment.
     *        Determine the expected segments count for the chunk using the offset and payload
     *        size.
     * 
     * @param segment Segment to be processed.
     * @param chunk reference to the Chunk to be assigned to.
     * @return true if a Chunk is complete as a result of processing the Segment.
     * @return false false otherwise.
     */
    bool Transport::process_segment(Chunk::Segment& segment, Chunk::Chunk& chunk)
    {
        uint8_t pos = insert_segment(segment);
        if (trackings[pos].check_complete())
        {
            chunk = trackings[pos];
            chunk.reassemble();
            trackings.remove(pos);
            return true;
        }

        return false; // Leave the reference Chunk untouch.
    }
    
    /**
     * @brief Inserts a Segment into a corresponding exisiting or new Chunk.
     * 
     * @param segment Segment to insert.
     * @return uint8_t index of inserted position in the Chunk trackings.
     */
    uint8_t Transport::insert_segment(Chunk::Segment& segment)
    {
        size_t count = trackings.size();
        for (uint8_t i = 0; i < count; i++)
        {
            if (trackings[i].get_id() == segment.chunk_id)
            {
                trackings[i].assemble_seg(segment);
                return i;
            }
        }

        // If reach here, the Chunk does not currently exist. Create a new Chunk.
        Chunk::Chunk new_chunk;
        new_chunk.set_id(segment.chunk_id);
        new_chunk.assemble_seg(segment);
        trackings.push_back(new_chunk);

        return trackings.size()-1;
    }

    /**
     * @brief Receives a data Chunk from a source address.
     * 
     * @param recv reference to a Chunk object to be assigned with received Chunk.
     * @return true if a Chunk is completed and that is assigned to the reference parameter.
     * @return false if no Chunks are completed and thus not assigned to reference parameter.
     */
    bool Transport::receive(Chunk::Chunk& recv)
    {
        uint8_t source = 0;
        uint8_t buf[SEGMENT_SIZE] = {0};
        uint8_t len = sizeof(buf);
        if (net_manager->recvfromAck(buf, &len, &source))
        {
            if (CRC::CRC::check_crc16(buf, SEGMENT_SIZE, CRC_IDX))
            {
                Chunk::Segment new_segment;
                new_segment.expand_seg(buf);
                return process_segment(new_segment, recv);
            }
        }

        return false; // Leave the reference Chunk untouch.
    }
}


