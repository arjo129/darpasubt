#include <circular_buffer.h>

/**
 * @brief Construct a new circular buffer::circular buffer object.
 * 
 * @param size Maximum size of circular buffer.
 */
circular_buffer::circular_buffer(size_t size) :
    buf_(new Chunk::Chunk[size]),
    head_(0),
    tail_(0),
    max_size_(size),
    full_(false)
{}

/**
 * @brief Destroy the circular buffer::circular buffer object.
 * 
 */
circular_buffer::~circular_buffer(void)
{
    delete[] buf_;
}

/**
 * @brief Puts one Chunk into the head of the circular buffer.
 * 
 * @param chunk the Chunk to put.
 */
void circular_buffer::put(Chunk::Chunk chunk)
{
    buf_[head_] = chunk; // Place or overwrite with the new Chunk.
    if (full_)
    {
        // Since it was to begin with, increase the tail pointer.
        tail_ = (tail_ + 1) % max_size_;
    }
    head_ = (head_ + 1) % max_size_; // Always increase head pointer.
    full_ = head_ == tail_; // Update the full_ flag.
}

/**
 * @brief Retrieves one Chunk from the tail of the circular buffer.
 * 
 * @return Chunk::Chunk 
 */
Chunk::Chunk circular_buffer::get(void)
{
    if (empty())
    {
        return Chunk::Chunk();
    }

    Chunk::Chunk ret = buf_[tail_];
    tail_ = (tail_ + 1) % max_size_;
    full_ = false;

    return ret;
}

/**
 * @brief Resets the circular buffer to empty buffer.
 * 
 */
void circular_buffer::reset(void)
{
    head_ = tail_;
    full_ = false;
}

/**
 * @brief Check if circular buffer is empty.
 * 
 * @return true if empty.
 * @return false if not empty.
 */
bool circular_buffer::empty(void) const
{
    return (!full_ && (head_ ==  tail_));
}

/**
 * @brief Check if circular buffer is full.
 * 
 * @return true if full.
 * @return false if empty.
 */
bool circular_buffer::full(void) const
{
    return full_;
}

/**
 * @brief Returns the maximum size of circular buffer.
 * 
 * @return size_t maximum size of circular buffer.
 */
size_t circular_buffer::capacity(void) const
{
    return max_size_;
}

/**
 * @brief Returns the number of elements in circular buffer.
 * 
 * @return size_t number of elements in circular buffer.
 */
size_t circular_buffer::size(void) const
{
    size_t size = max_size_;

    if (!full_)
    {
        if (head_ >= tail_)
        {
            size = head_ - tail_;
        }
        else
        {
            size = max_size_ + head_ - tail_;
        }
    }

    return size;
}