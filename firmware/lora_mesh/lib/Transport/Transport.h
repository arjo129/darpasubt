#ifndef Transport_h
#define Transport_h

#include <Arduino.h>
#include <RHMesh.h>
#include <Array.h>
#include <circular_buffer.h>
#include <common.h>

namespace Transport
{
    const size_t MAX_BUFFER_SIZE = MAX_CHUNK_QUEUE_SIZE;
    const uint8_t MAX_ATTEMPTS = MAX_CHUNK_ATTEMPTS;
    
    class Transport
    {
        private:
            uint8_t src_addr;
            RHMesh* net_manager;
            circular_buffer send_queue;
            // circular_buffer recv_queue;
            Array<Chunk::Chunk, MAX_BUFFER_SIZE> trackings;
            
            bool send(Chunk::Chunk& chunk);
            bool process_segment(Chunk::Segment& segment, Chunk::Chunk& chunk);
            uint8_t insert_segment(Chunk::Segment& segment);
            
        public:
            Transport(uint8_t src_addr, RHMesh* net_manager);
            ~Transport(void);
            void queue_chunk(Chunk::Chunk chunk);
            void process_send_queue(void);
            bool receive(Chunk::Chunk& recv);
    };
}

#endif