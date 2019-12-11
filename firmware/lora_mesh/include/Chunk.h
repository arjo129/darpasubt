#include <Arduino.h>
#include <crc.h>

#define HEADER_SIZE 6
#define PAYLOAD_SIZE 32
#define SEGMENT_SIZE (HEADER_SIZE) + (PAYLOAD_SIZE)
#define CHUNK_IDX 0
#define F_OFFSET_IDX 2
#define CRC_IDX 4
#define PAYL_IDX 6

// NOTE: Tested 150, its stretching SRAM limit.
#define MAX_DATA_SIZE 300
#define MAX_SEG_CNT (((MAX_DATA_SIZE) / (PAYLOAD_SIZE)) + 1)

#define SYN_BIT 0x4000
#define MF_BIT 0x2000
#define OFFSET_SET_MASK 0x1FFF

namespace Chunk
{
    /**
     * @brief Structure to represent a single Segment.
     * 
     * flags:
     *      |*****|*****|*****|*****|*****|*****|*****|*****|
     *      | ACK | ACK | ACK | ACK | ACK | ACK | ACK | ACK |
     *      |*****|*****|*****|*****|*****|*****|*****|*****|
     */
    struct Segment 
    {
        uint16_t chunk_id;
        uint16_t flags_offset;
        uint8_t payload[PAYLOAD_SIZE];
        Segment(void);
        ~Segment(void);
    };

    class Chunk
    {
        private:
            uint16_t id;
            uint8_t data[MAX_DATA_SIZE];
            uint16_t len;
            Segment segments[MAX_SEG_CNT];
            uint8_t segments_count;

        public:
            Chunk(void);
            Chunk(uint8_t* data, uint16_t len);
            ~Chunk(void);
            uint16_t get_seg_count(void);
            void segment_data(uint8_t& src_addr, uint8_t& dest_addr);
            void flatten_seg(uint8_t index, uint8_t buf[SEGMENT_SIZE]);
    };
}