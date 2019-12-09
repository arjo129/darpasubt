#include <Arduino.h>
#include <RHMesh.h>
#include <Chunk.h>

namespace Transport
{
    class Transport
    {
        private:
            uint8_t source_addr;
            Chunk::Chunk* chunk;
            RHMesh* net_manager;
            
        public:
            Transport(uint8_t src_addr, RHMesh* net_manager);
            ~Transport(void);
            void send(uint8_t dest_addr, Chunk::Chunk* chunk);
            void receive(uint8_t* data);
    };
}