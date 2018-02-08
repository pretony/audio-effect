#ifndef SRC_OUTPUT_OUTPUTPLUGIN_H_
#define SRC_OUTPUT_OUTPUTPLUGIN_H_

#include "../Common/AudioFormat.h"
#include <string>


enum class OutputType
{
    UNDEFINED = 0,
    ALSAOUTPUT,
};

class Effect;
class OutputPlugin
{
public:
    OutputPlugin(Effect &_effect):effect(_effect),is_open(false)
    {

    }
    virtual ~OutputPlugin()
    {

    }
    AudioFormat audio_format;
    Effect &effect;

    virtual void init() = 0;
    virtual bool open() = 0;
    virtual bool setup() = 0;
    virtual void close() = 0;
    virtual size_t write(void *ptr, size_t size) = 0;
    virtual void start() = 0;

    bool getIsOpen() const
    {
        return is_open;
    }

    void setIsOpen(bool _is_open)
    {
        is_open = _is_open;
    }

    void setAudioFormat(const AudioFormat& audioFormat)
    {
        audio_format = audioFormat;
    }

private:
    std::string url;
    bool is_open;

};

#endif /* SRC_OUTPUT_OUTPUTPLUGIN_H_ */
