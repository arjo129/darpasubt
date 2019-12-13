#include <Chunk.h>

namespace Chunk
{
    /**
     * @brief Construct a new Segment:: Segment struct.
     * 
     */
    Segment::Segment(void) : chunk_id(0), chunk_len(0), flags_offset(0)
    {
        for (int i = 0; i < PAYLOAD_SIZE; i++)
        {
            payload[i] = 0;
        }
    }

    Segment::~Segment(void) {}

    /**
     * @brief Flattens a Segment into a bytes array.
     * 
     * @param buf array to hold flattened Segment.
     */
    void Segment::flatten_seg(uint8_t buf[SEGMENT_SIZE])
    {
        buf[CHUNK_IDX] = (this->chunk_id & HIGH_16_BITS_MASK) >> 8;
        buf[CHUNK_IDX+1]  = this->chunk_id & LOW_16_BITS_MASK;
        buf[CHUNK_LEN_IDX] = (this->chunk_len & HIGH_16_BITS_MASK) >> 8;
        buf[CHUNK_LEN_IDX+1]  = this->chunk_len & LOW_16_BITS_MASK;
        buf[F_OFFSET_IDX] = (this->flags_offset & HIGH_16_BITS_MASK) >> 8;
        buf[F_OFFSET_IDX+1] = this->flags_offset;
        memcpy(&buf[PAYL_IDX], &(this->payload), PAYLOAD_SIZE);

        CRC::CRC::append_crc16(buf, SEGMENT_SIZE, CRC_IDX);
    }

    /**
     * @brief Expands a bytes array into a Segment.
     * 
     * @param buf buffer array containing the bytes.
     */
    void Segment::expand_seg(uint8_t buf[SEGMENT_SIZE])
    {
        this->chunk_id |= (uint16_t)buf[CHUNK_IDX] << 8;
        this->chunk_id |= (uint16_t)buf[CHUNK_IDX+1];
        this->chunk_len |= (uint16_t)buf[CHUNK_LEN_IDX] << 8;
        this->chunk_len |= (uint16_t)buf[CHUNK_LEN_IDX+1];
        this->flags_offset |= (uint16_t)buf[F_OFFSET_IDX] << 8;
        this->flags_offset |= (uint16_t)buf[F_OFFSET_IDX+1];
        memcpy(&(this->payload), &buf[PAYL_IDX], PAYLOAD_SIZE);
    }

    /**
     * @brief Operator == overload to check if two Segments are equal in attribute values.
     * 
     * @param seg Segment to compare with.
     * @return true if every attributes have the same value.
     * @return false if at least one attribute has different values.
     */
    bool Segment::operator==(const Segment& seg)
    {
        if (this->chunk_id == seg.chunk_id && 
            this->chunk_len == seg.chunk_len &&
            this->flags_offset == seg.flags_offset &&
            !memcmp(this->payload, seg.payload, PAYLOAD_SIZE))
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if the SYN bit is set.
     * 
     * @return true if SYN is set to '1'.
     * @return false if SYN is not set to '0'.
     */
    bool Segment::check_syn(void)
    {
        return (this->flags_offset & SYN_BIT) > 0;
    }

    /**
     * @brief Checks if the MF bit is set.
     * 
     * @return true if MF is set to '1'.
     * @return false if MF is not set '0'.
     */
    bool Segment::check_mf(void)
    {
        return (this->flags_offset & MF_BIT) > 0;
    }

    /**
     * @brief Retrieves the Segment offset.
     * 
     * @return uint16_t offset value.
     */
    uint16_t Segment::get_offset(void)
    {
        return (this->flags_offset & OFFSET_SET_MASK);
    }

    /**
     * @brief Construct a new Chunk:: Chunk object.
     * 
     * @param data bytes array to store as a Chunk.
     * @param len length of bytes array (in bytes).
     */
    Chunk::Chunk(uint8_t* data, uint16_t len, uint16_t dest) : 
        len(len), 
        segments_count(0), 
        dest(dest),
        attempts(0)
    {
        memset(this->data, 0, MAX_DATA_SIZE);
        memset(this->segments, 0, sizeof(Segment) * MAX_SEG_CNT);
        for (uint16_t i = 0; i < len; i++)
        {
            this->data[i] = data[i];
        }
        this->id = gen_id(micros());
    }

    /**
     * @brief Construct a new Chunk:: Chunk object.
     * 
     */
    Chunk::Chunk(void) :
    id(0),
    len(0),
    segments_count(0),
    dest(0),
    attempts(0)
    {
        memset(this->data, 0, MAX_DATA_SIZE);
        memset(this->segments, 0, sizeof(Segment) * MAX_SEG_CNT);
    }

    Chunk::~Chunk(void) {}

    /**
     * @brief Sets the id of the Chunk.
     * 
     */
    void Chunk::set_id(uint16_t id)
    {
        this->id = id;
    }

    /**
     * @brief Increment the attempts counter by one.
     * 
     */
    void Chunk::increment_attempts(void)
    {
        this->attempts++;
    }

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
     * @brief Returns a pointer to the data field of the Chunk.
     * 
     * @return uint8_t* pointer to the first byte of data.
     */
    uint8_t* Chunk::get_data(void)
    {
        return this->data;
    }

    /**
     * @brief Returns the length of data in the Chunk.
     * 
     * @return uint16_t length in bytes.
     */
    uint16_t Chunk::get_len(void)
    {
        return this->len;
    }

    /**
     * @brief Returns the number of attempts.
     * 
     * @return uint8_t number of attempts.
     */
    uint8_t Chunk::get_attempts(void)
    {
        return this->attempts;
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
            seg.chunk_len = this->len;
            
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
            
            if (len - byte_idx < PAYLOAD_SIZE) pl_len = len - byte_idx;
            memcpy(&(seg.payload), &data[i * PAYLOAD_SIZE], pl_len);

            this->segments[i] = seg;
        }
    }

    /**
     * @brief Assemble a Segment back into the Chunk.
     * 
     * @param segment Segment to assemble back.
     */
    void Chunk::assemble_seg(Segment segment)
    {
        uint16_t pos = (segment.get_offset() * 8) / PAYLOAD_SIZE;
        if (this->segments[pos] == segment) return;

        this->segments[pos] = segment;
        segments_count++;
    }

    /**
     * @brief Check if Chunk is complete and has all original Segments.
     * 
     * @return true if Chunk has all Segments.
     * @return false if Chunk has at least one missing Segment.
     */
    bool Chunk::check_complete(void)
    {
        if (segments_count == 0)
            return false; // Early return.

        // Look for the last Segment from the back.
        uint8_t i;
        for (i = MAX_SEG_CNT-1; i >= 0; i--)
        {
            Segment seg = segments[i];
            if (!seg.check_mf() && seg.get_offset() == 0 && this->segments_count == 1)
            {
                // This is the first and only Segment in this Chunk.
                return true;
            }

            if (!seg.check_mf() && seg.get_offset() > 0)
            {
                // Found the last Segment.
                uint16_t supposed_count = ((seg.get_offset() * 8) / PAYLOAD_SIZE) + 1;
                if (this->segments_count == supposed_count) 
                    return true;
                else 
                    return false;
            }
        }

        // If reach here, it means no last or any Segments are found at all.
        return false;
    }

    /**
     * @brief Reassemble the Chunk with it's own Segments.
     * 
     */
    void Chunk::reassemble(void)
    {
        for (int i = 0; i < this->segments_count; i++)
        {
            memcpy(&(this->data[i * PAYLOAD_SIZE]), this->segments[i].payload, PAYLOAD_SIZE);
        }

        this->id = this->segments[0].chunk_id;
        this->len = this->segments[0].chunk_len;
    }

    /**
     * @brief Retrieve a flattened Segment in the given index position.
     * 
     * @param index index position of Segment stored.
     * @param flat pointer to buffer to store flattened Segment.
     */
    void Chunk::get_flat_segment(uint16_t index, uint8_t flat[SEGMENT_SIZE])
    {
        this->segments[index].flatten_seg(flat);
    }

    /**
     * @brief Generates a random using a seed.
     * 
     * @param seed seed to use.
     * @return uint16_t generated random number.
     */
    uint16_t Chunk::gen_id(uint32_t seed)
    {
        randomSeed(seed);
        return random(sizeof(this->id));
    }
}