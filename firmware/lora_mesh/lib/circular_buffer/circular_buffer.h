#ifndef circular_buffer_h
#define circular_buffer_h

#include <stdlib.h>
#include <Chunk.h>

class circular_buffer
{
    public:
        explicit circular_buffer(size_t size);
        ~circular_buffer(void);

        void put(Chunk::Chunk chunk);
        Chunk::Chunk get(void);
        void reset(void);
        bool empty(void) const;
        bool full(void) const;
        size_t capacity(void) const;
        size_t size(void) const;

    private:
        Chunk::Chunk* buf_;        
        size_t head_;
        size_t tail_;
        const size_t max_size_;
        bool full_;  
};

#endif