#ifndef Chunk_h
#define Chunk_h

#include <Arduino.h>
#include <crc.h>

#define HEADER_SIZE 8
#define PAYLOAD_SIZE 32
#define SEGMENT_SIZE (HEADER_SIZE) + (PAYLOAD_SIZE)
#define CHUNK_IDX 0
#define CHUNK_LEN_IDX 2
#define F_OFFSET_IDX 4
#define CRC_IDX 6
#define PAYL_IDX 8

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
 *      |                 CHUNK_LEN (16)                |
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
    class Segment 
    {
        public:
        uint16_t chunk_id;
        uint16_t chunk_len;
        uint16_t flags_offset;
        uint8_t payload[PAYLOAD_SIZE];

        Segment(void);
        ~Segment(void);
        void flatten_seg(uint8_t buf[SEGMENT_SIZE]);
        bool check_syn(void);
        bool check_mf(void);
        uint16_t get_offset(void);
        void expand_seg(uint8_t buf[SEGMENT_SIZE]);

        bool operator==(const Segment& seg);
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
        uint8_t attempts;

        public:
        Chunk(void);
        Chunk(uint8_t* data, uint16_t len, uint16_t dest);
        ~Chunk(void);
        
        void set_id(uint16_t id);
        void increment_attempts(void);
        uint16_t get_seg_count(void);
        uint16_t get_dest(void);
        uint16_t get_id(void);
        uint8_t* get_data(void);
        uint16_t get_len(void);
        uint8_t get_attempts(void);

        void segment_data(uint8_t& src_addr);
        void assemble_seg(Segment segment);
        bool check_complete(void);
        void reassemble(void);
        void get_flat_segment(uint16_t index, uint8_t flat[SEGMENT_SIZE]);
    };
}

#endif