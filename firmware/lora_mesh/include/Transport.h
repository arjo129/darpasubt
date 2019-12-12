#include <Arduino.h>
#include <RHMesh.h>
#include <Array.h>
#include <circular_buffer.h>

namespace Transport
{
    const size_t MAX_BUFFER_SIZE = 1;
    
    class Transport
    {
        private:
            uint8_t src_addr;
            RHMesh* net_manager;
            circular_buffer send_queue;
            // circular_buffer recv_queue;
            Array<Chunk::Chunk, MAX_BUFFER_SIZE> trackings;
            
            void send(Chunk::Chunk& chunk);
            Chunk::Chunk process_segment(Chunk::Segment& segment, bool& complete);
            uint8_t insert_segment(Chunk::Segment& segment);
            
        public:
            Transport(uint8_t src_addr, RHMesh* net_manager);
            ~Transport(void);
            void queue_chunk(Chunk::Chunk chunk);
            void process_send_queue(void);
            Chunk::Chunk receive(bool& complete);
    };
}