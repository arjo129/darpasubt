#include <Chunk.h>

namespace Chunk
{
    /**
     * @brief Construct a new Segment:: Segment struct.
     * 
     */
    Segment::Segment(void) : chunk_id(0), flags_offset(0)
    {
        for (int i = 0; i < PAYLOAD_SIZE; i++)
        {
            payload[i] = 0;
        }
    }

    Segment::~Segment(void) {}

    /**
     * @brief Construct a new Chunk:: Chunk object.
     * 
     * @param data bytes array to store as a Chunk.
     * @param len length of bytes array (in bytes).
     */
    Chunk::Chunk(uint8_t* data, uint16_t len, uint16_t dest) : 
        len(len), 
        segments_count(0), 
        dest(dest)
    {
        memset(this->data, 0, MAX_DATA_SIZE);
        memset(this->segments, 0, sizeof(Segment) * MAX_SEG_CNT);
        for (uint16_t i = 0; i < len; i++)
        {
            this->data[i] = data[i];
        }
        this->id = CRC::CRC::calc_crc16(this->data, MAX_DATA_SIZE);
    }

    /**
     * @brief Construct a new Chunk:: Chunk object.
     * 
     */
    Chunk::Chunk(void) :
    id(0),
    len(0),
    segments_count(0),
    dest(0)
    {
        memset(this->data, 0, MAX_DATA_SIZE);
        memset(this->segments, 0, sizeof(Segment) * MAX_SEG_CNT);
    }

    Chunk::~Chunk(void) {}

    /**
     * @brief Returns the number of Segments in this Chunk.
     * 
     * @return uint16_t number of Segments.
     */
    uint16_t Chunk::get_seg_count(void)
    {
        return this->segments_count;
    }

    /**
     * @brief Returns the destination address of this Chunk.
     * 
     * @return uint16_t destination address.
     */
    uint16_t Chunk::get_dest(void)
    {
        return this->dest;
    }

    /**
     * @brief Returns the Chunk id.
     * 
     * @return uint16_t Chunk id.
     */
    uint16_t Chunk::get_id(void)
    {
        return this->id;
    }

    /**
     * @brief Breaks a Chunk into Segments.
     * 
     * @param src_addr address of source node.
     */
    void Chunk::segment_data(uint8_t& src_addr)
    {
        this->segments_count = (len + PAYLOAD_SIZE - 1) / PAYLOAD_SIZE;
        
        for (uint8_t i = 0; i < this->segments_count; i++)
        {
            Segment seg;

            seg.chunk_id = this->id;
            // First Segment
            if (i == 0)
            {
                seg.flags_offset |= SYN_BIT;
            }

            // Last Segment
            if (i != this->segments_count - 1)
            {
                seg.flags_offset |= MF_BIT;
            }

            uint16_t pl_len = PAYLOAD_SIZE;
            uint16_t byte_idx = i * PAYLOAD_SIZE;

            seg.flags_offset |= (OFFSET_SET_MASK & (byte_idx / 8)); // Segment offset
            Serial.println(seg.flags_offset, HEX);
            
            if (len - byte_idx < PAYLOAD_SIZE) pl_len = len - byte_idx;
            memcpy(&(seg.payload), &data[i * PAYLOAD_SIZE], pl_len);

            this->segments[i] = seg;
        }
    }

    /**
     * @brief Flattens a Segment into a bytes array.
     * 
     * @param index index position of the segment to flatten.
     * @param buf array to hold flattened Segment.
     */
    void Chunk::flatten_seg(uint8_t index, uint8_t buf[SEGMENT_SIZE])
    {
        buf[CHUNK_IDX] = (this->id & HIGH_16_BITS_MASK) >> 8;
        buf[CHUNK_IDX+1]  = this->id & LOW_16_BITS_MASK;
        buf[F_OFFSET_IDX] = (this->segments[index].flags_offset & HIGH_16_BITS_MASK) >> 8;
        buf[F_OFFSET_IDX+1] = this->segments[index].flags_offset;
        memcpy(&buf[PAYL_IDX], &(this->segments[index].payload), PAYLOAD_SIZE);

        CRC::CRC::append_crc16(buf, SEGMENT_SIZE, CRC_IDX);
    }

    /**
     * @brief Assemble a Segment back into the Chunk.
     * 
     * @param segment Segment to assemble back.
     * @return true if Segment is first Segment or belongs to the Chunk.
     * @return false if Segment does not belong to the Chunk and is not assembled.
     */
    bool Chunk::assemble_seg(Segment segment)
    {
        if (check_syn(segment.flags_offset))
        {
            this->id = segment.chunk_id;
            this->segments[0] = segment;
            segments_count++;
        }
        else
        {
            if (segment.chunk_id != this->id)
            {
                return false;
            }

            uint16_t pos = (get_offset(segment.flags_offset) * 8) / PAYLOAD_SIZE;
            this->segments[pos] = segment;
            segments_count++;
        }

        return true;
    }

    /**
     * @brief Reassemble the Chunk with it's own Segments.
     * 
     */
    void Chunk::reassemble(void)
    {
        for (int i = 0; i < this->segments_count; i++)
        {
            memcpy(&(this->data[i * PAYLOAD_SIZE]), segments[i].payload, PAYLOAD_SIZE);
        }

        this->id = CRC::CRC::calc_crc16(this->data, MAX_DATA_SIZE);
    }

    /**
     * @brief Expands a bytes array into a Segment.
     * 
     * @param buf buffer array containing the bytes.
     * @return Segment expanded Segment.
     */
    Segment Chunk::expand_seg(uint8_t buf[SEGMENT_SIZE])
    {
        Segment ret;

        ret.chunk_id |= (uint16_t)buf[CHUNK_IDX] << 8;
        ret.chunk_id |= (uint16_t)buf[CHUNK_IDX+1];
        ret.flags_offset |= (uint16_t)buf[F_OFFSET_IDX] << 8;
        ret.flags_offset |= (uint16_t)buf[F_OFFSET_IDX+1];
        memcpy(&ret.payload, &buf[PAYL_IDX], PAYLOAD_SIZE);

        return ret;
    }

    /**
     * @brief Checks if the SYN bit is set.
     * 
     * @param flags_offset the bit field containing the SYN bit.
     * @return true if SYN is set.
     * @return false if SYN is not set.
     */
    bool Chunk::check_syn(uint16_t flags_offset)
    {
        return (flags_offset & SYN_BIT) > 0;
    }

    /**
     * @brief Checks if the MF bit is set.
     * 
     * @param flags_offset the bit field containing the MF bit.
     * @return true if MF is set.
     * @return false if MF is not set.
     */
    bool Chunk::check_mf(uint16_t flags_offset)
    {
        return (flags_offset & MF_BIT) > 0;
    }

    /**
     * @brief Retrieves the Segment offset.
     * 
     * @param flags_offset the bit field containing the offset bits.
     * @return uint16_t 
     */
    uint16_t Chunk::get_offset(uint16_t flags_offset)
    {
        return (flags_offset & OFFSET_SET_MASK);
    }
}