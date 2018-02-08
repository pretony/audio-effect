#include "Plugins/AlsaInput.h"
#include "../Effect/EffectApi.h"
#include "../Common/Ringbuf.h"
#include "../Common/Util.h"
#include <memory>

static std::unique_ptr<InputPlugin> getInputPtr(Effect &effect, InputType inputType)
{
    switch(inputType)
    {
    case InputType::ALSAINPUT:
        return std::unique_ptr<InputPlugin>(new AlsaInput(effect));
    default:
        return nullptr;
    }
    return nullptr;
}

void inputThread(Effect &effect, InputType inputType)
{
    std::unique_ptr<InputPlugin> inputPlugin = getInputPtr(effect, inputType);
    if(inputPlugin == nullptr)
    {
        printf("create input plugin failed, inputType=%d\n", as_integer(inputType));
        return;
    }
    inputPlugin->init();
    if(!inputPlugin->open())
    {
        printf("input open failed\n");
        return;
    }
    if(!inputPlugin->setup())
    {
        printf("input setup failed\n");
        return;
    }
    unsigned int nfds = inputPlugin->getFileDescriptors();
    std::unique_ptr<struct pollfd> pfds(new struct pollfd[nfds]);
    inputPlugin->getPollFd(pfds.get());
    inputPlugin->start();
    ringbuf_t rb = ringbuf_new(4096);
    printf("input plugin open ok\n");

    while(1)
    {
        poll(pfds.get(), nfds, -1);
        if(inputPlugin->checkPolling(pfds.get()))
        {
            size_t calBytes = 0;
            uint8_t buffer[4096]={0};
            size_t nbytes = inputPlugin->read(buffer, ringbuf_bytes_free(rb));
            if(nbytes == 0)
            {
                continue;
            }
            ringbuf_memcpy_into(rb, buffer, nbytes);

            //获取最大2*2倍数的字节数
            size_t usedBytes = ringbuf_bytes_used(rb);
            size_t mod = usedBytes%(2*2);//2是16bit，2是声道数
            calBytes = usedBytes - mod;
            memset(buffer, 0x00, sizeof(buffer));
            ringbuf_memcpy_from(buffer, rb, calBytes);
            copyToEffect(inputPlugin->effect, buffer, calBytes);
        }
    }
    ringbuf_free(&rb);
}
