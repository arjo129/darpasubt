#ifndef serial_reader_h
#define serial_reader_h

#include <Arduino.h>
#include <circular_buffer.h>
#include <common.h>


namespace serial_reader
{
    const uint8_t DEST_ADDR_SIZE = sizeof(uint8_t);
    const uint16_t QUEUE_SIZE = 10;
    const uint16_t SERIAL_READ_SIZE = MAX_DATA_SIZE + DEST_ADDR_SIZE;
    const uint8_t RECV_LEN_SIZE = 2;
    const char BEGIN_CHAR = '<';
    const char END_CHAR = '>';

    extern bool receiving_len;
    extern bool receiving_body;
    extern circular_buffer read_queue;
    extern uint8_t buffer[SERIAL_READ_SIZE];
    extern uint16_t current_len;
    extern uint16_t recv_len;
    extern uint8_t recv_len_check_count;
    
    void read(void);
    void clear(void);
}

#endif