#ifndef SRC_EFFECT_EFFECTBUFFER_H_
#define SRC_EFFECT_EFFECTBUFFER_H_

#include <mutex>
#include <condition_variable>
#include <pthread.h>
#include "../Common/Ringbuf.h"

enum class EffectBufferCmd
{
    NONE,
    START,
    STOP,
};

class EffectBuffer
{
public:
    EffectBuffer(int _max_size = 8192):max_size(_max_size),cmd(EffectBufferCmd::NONE)
    {
        //TODO:use unique_ptr later
        rb = ringbuf_new(_max_size);
    }
    ~EffectBuffer()
    {
        ringbuf_free(&rb);
    }

    void push(const void *data, size_t length)
    {
        std::unique_lock<std::mutex> lock(mutex);
        cond_notFull.wait(lock, [this](){return (!ringbuf_is_full(rb) && (cmd != EffectBufferCmd::NONE)); });
        ringbuf_memcpy_into(rb, data, length);
        cond_notEmpty.notify_one();
    }

    bool pull(void *data, size_t length)
    {
        std::unique_lock<std::mutex> lock(mutex);
        cond_notEmpty.wait(lock, [this](){return (!ringbuf_is_empty(rb) && (cmd != EffectBufferCmd::NONE)); });
        void *result = ringbuf_memcpy_from(data, rb, length);
        cond_notFull.notify_one();
        return (result == 0) ? false : true;
    }

    size_t get_max_size()
    {
        return max_size;
    }

    void set_max_size(size_t _max_size)
    {
        max_size = _max_size;
    }

    ringbuf_t get_rb()
    {
        return rb;
    }

    pthread_mutex_t *get_mutex()
    {
        return static_cast<pthread_mutex_t *>(mutex.native_handle());
    }

    pthread_cond_t *get_cond_notFull()
    {
        return static_cast<pthread_cond_t *>(cond_notFull.native_handle());
    }

    pthread_cond_t *get_cond_notEmpty()
    {
        return static_cast<pthread_cond_t *>(cond_notEmpty.native_handle());
    }

private:
    size_t max_size;
    std::mutex mutex;
    std::condition_variable cond_notFull;
    std::condition_variable cond_notEmpty;
    ringbuf_t rb;
    EffectBufferCmd cmd;
};

#endif /* SRC_EFFECT_EFFECTBUFFER_H_ */
