#include "Plugins/AlsaOutput.h"
#include "../Effect/EffectApi.h"
#include "../Common/Util.h"
#include <memory>
#include <vector>

static std::unique_ptr<OutputPlugin> getOutputPtr(Effect &effect, OutputType outputType)
{
    switch(outputType)
    {
    case OutputType::ALSAOUTPUT:
        return std::unique_ptr<OutputPlugin>(new AlsaOutput(effect));
    default:
        return nullptr;
    }
    return nullptr;
}

void outputThread(Effect &effect, OutputType outputType)
{
    std::unique_ptr<OutputPlugin> outputPlugin = getOutputPtr(effect, outputType);
    if(outputPlugin == nullptr)
    {
        printf("create output plugin failed, outputType=%d\n", as_integer(outputType));
        return;
    }
    outputPlugin->init();
    if(!outputPlugin->open())
    {
        printf("output open failed\n");
        return;
    }
    if(!outputPlugin->setup())
    {
        printf("output setup failed\n");
        return;
    }
    outputPlugin->start();
    printf("output plugin open ok\n");

    while(1)
    {
        size_t length = outputPlugin->effect.effect_out_buffer.get_max_size()/2;
        std::vector<uint8_t> dataV(length);
        if(copyFromEffect(outputPlugin->effect, dataV.data(), length))
        {
            uint8_t *write_data = dataV.data();
            while (length > 0)
            {
                size_t nbytes;
                nbytes = outputPlugin->write(write_data, length);
                if (nbytes == 0)
                {
                    printf("outputPlugin write error\n");
                    return;
                }
                write_data += nbytes;
                length -= nbytes;
            }
        }
    }
}
