#ifndef serial_reader_h
#define serial_reader_h

#include <Arduino.h>
#include <common.h>


namespace serial_reader
{
    const char TERMINATE_CHAR = '\n';
    extern uint8_t buffer[SERIAL_READ_SIZE];
    extern uint16_t current_len;
    extern bool terminate;
    
    void read(void);
    void clear(void);
}

#endif