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
#define MAX_DATA_SIZE 50
#define MAX_SEG_CNT (((MAX_DATA_SIZE) / (PAYLOAD_SIZE)) + 1)

#define MF_BIT 0x2000
#define SYN_BIT 0x4000
#define HIGH_16_BITS_MASK 0xFF00
#define LOW_16_BITS_MASK 0x00FF
#define OFFSET_SET_MASK 0x1FFF

/**
 *      Segment representation:
 *      * The number of bits of each value is given in parentheses (execept for flags).
 * 
 *      |*****|*****|*****|*****|*****|*****|*****|*****|
 *      |                 CHUNK_ID (16)                 |
 *      |*****|*****|*****|*****|*****|*****|*****|*****|
 *      |RSV 1|SYN 1| MF 1|        OFFSET (13)          |
 *      |*****|*****|*****|*****|*****|*****|*****|*****|
 *      |                    CRC (16)                   |
 *      |*****|*****|*****|*****|*****|*****|*****|*****|
 *      |                 PAYLOAD (VARY)                |
 *      |*****|*****|*****|*****|*****|*****|*****|*****|
 */

namespace Chunk
{
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
            uint16_t dest;

        public:
            Chunk(void);
            Chunk(uint8_t* data, uint16_t len, uint16_t dest);
            ~Chunk(void);
            
            uint16_t get_seg_count(void);
            uint16_t get_dest(void);
            uint16_t get_id(void);

            void segment_data(uint8_t& src_addr);
            void flatten_seg(uint8_t index, uint8_t buf[SEGMENT_SIZE]);
            bool assemble_seg(Segment segment);
            void reassemble(void);
            
            static Segment expand_seg(uint8_t buf[SEGMENT_SIZE]);
            static bool check_syn(uint16_t flags_offset);
            static bool check_mf(uint16_t flags_offset);
            static uint16_t get_offset(uint16_t flags_offset);
    };
}