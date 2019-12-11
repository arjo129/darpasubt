#include <Arduino.h>
#include <RHMesh.h>
#include <circular_buffer.h>

namespace Transport
{
    const size_t MAX_BUFFER_SIZE = 1;
    
    class Transport
    {
        private:
            uint8_t src_addr;
            RHMesh* net_manager;
            circular_buffer queue;
            
            void send(Chunk::Chunk chunk);
            
        public:
            Transport(uint8_t src_addr, RHMesh* net_manager);
            ~Transport(void);
            void queue_chunk(Chunk::Chunk& chunk);
            void process_queue(void);
            Chunk::Chunk receive(void);
    };
}