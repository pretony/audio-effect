#ifndef SRC_INPUT_INPUTPLUGIN_H_
#define SRC_INPUT_INPUTPLUGIN_H_

#include "../Common/AudioFormat.h"
#include <string>
#include <poll.h>

enum class InputType
{
    UNDEFINED = 0,
    ALSAINPUT,
};

class Effect;
class InputPlugin
{
public:
    InputPlugin(Effect &_effect):effect(_effect),is_open(false)
    {

    }
    virtual ~InputPlugin()
    {

    }
    AudioFormat audio_format;
    Effect &effect;

    virtual void init() = 0;
    virtual bool open() = 0;
    virtual bool setup() = 0;
    virtual void close() = 0;
    virtual size_t read(void *ptr, size_t size) = 0;
    virtual void start() = 0;
    virtual unsigned int getFileDescriptors() = 0;
    virtual void getPollFd(struct pollfd *_pfds) = 0;
    virtual int checkPolling(struct pollfd *_pfds) = 0;

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

#endif /* SRC_INPUT_INPUTPLUGIN_H_ */
