#ifndef SRC_OUTPUT_PLUGINS_ALSAOUTPUT_H_
#define SRC_OUTPUT_PLUGINS_ALSAOUTPUT_H_

#include "../OutputPlugin.h"
#include <alsa/asoundlib.h>

class AlsaOutput : public OutputPlugin
{
public:
    AlsaOutput(Effect &_effect)
        : OutputPlugin(_effect),pcm_handle(nullptr)
    {
    }
    ~AlsaOutput()
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
    size_t write(void *ptr, size_t size);
    void start();

private:
    std::string hw_device;
    snd_pcm_t *pcm_handle;
};

#endif /* SRC_OUTPUT_PLUGINS_ALSAOUTPUT_H_ */
