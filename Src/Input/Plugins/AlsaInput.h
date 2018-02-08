#ifndef SRC_INPUT_PLUGINS_ALSAINPUT_H_
#define SRC_INPUT_PLUGINS_ALSAINPUT_H_

#include "../InputPlugin.h"
#include <alsa/asoundlib.h>

class AlsaInput : public InputPlugin
{
public:
    AlsaInput(Effect &_effect)
        : InputPlugin(_effect),pcm_handle(nullptr),nfds(0),pfds(nullptr)
    {
    }
    ~AlsaInput()
    {
    }
    void init()
    {
        hw_device = "hw:0,0";
        audio_format.setSampleRate(44100);
        audio_format.setBitsPerSample(16);
        audio_format.setChannels(2);
    }
    bool open();
    bool setup();
    void close();
    size_t read(void *ptr, size_t size);
    void start();
    unsigned int getFileDescriptors();
    void getPollFd(struct pollfd *_pfds);
    int checkPolling(struct pollfd *_pfds);

private:
    std::string hw_device;
    snd_pcm_t *pcm_handle;
    unsigned int nfds;
    struct pollfd* pfds;
};

#endif /* SRC_INPUT_PLUGINS_ALSAINPUT_H_ */
