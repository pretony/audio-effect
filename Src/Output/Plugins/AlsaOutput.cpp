#include "AlsaOutput.h"
#include <chrono>
#include <errno.h>
#include <thread>

bool AlsaOutput::open()
{
    int ret = 0;
    for(int try_count = 0; try_count < 5 && -EBUSY == ret; try_count++)
    {
        ret = snd_pcm_open(&pcm_handle, hw_device.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(ret < 0)
    {
        printf("open device:%s failed, result(%d), result string(%s)\n", hw_device.c_str(), ret, snd_strerror(ret));
        return false;
    }

    return true;
}

//获得实际要设置声卡的采样比特深度
static snd_pcm_format_t getAlsaFormat(int bits_per_sample)
{
    switch (bits_per_sample) {
    case 8:
        return SND_PCM_FORMAT_S8;
    case 16:
        return SND_PCM_FORMAT_S16;
    case 24:
        return SND_PCM_FORMAT_S24;
    case 32:
        return SND_PCM_FORMAT_S32;
    default :
        return SND_PCM_FORMAT_UNKNOWN;
    }
}

bool AlsaOutput::setup()
{
    snd_pcm_hw_params_t* hw_params;
    snd_pcm_hw_params_alloca(&hw_params);
    int ret = 0;
    if ((ret = snd_pcm_hw_params_any(pcm_handle, hw_params)) < 0)
    {
        printf("Cannot initialize hardware parameter structure (%s)\n", snd_strerror(ret));
        return false;
    }
    if ((ret = snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        printf("Cannot set access type (%s)\n", snd_strerror (ret));
        return false;
    }
    snd_pcm_format_t alsa_format = getAlsaFormat(audio_format.getBitsPerSample());
    if ((ret = snd_pcm_hw_params_set_format(pcm_handle, hw_params, alsa_format)) < 0)
    {
        printf("Cannot set sample format (%s)\n", snd_strerror (ret));
        return false;
    }
    unsigned int channels = audio_format.getChannels();
    if ((ret = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, channels)) < 0)
    {
        printf("Cannot set channels (%s)\n", snd_strerror (ret));
        return false;
    }
    unsigned int sample_rate = audio_format.getSampleRate();
    if ((ret = snd_pcm_hw_params_set_rate(pcm_handle, hw_params, sample_rate, 0)) < 0)
    {
        printf("Cannot set sample rate (%s)\n", snd_strerror (ret));
        return false;
    }
    unsigned int periods = 2;
    int dir = 0;
    if ((ret = snd_pcm_hw_params_set_periods_near(pcm_handle, hw_params,  &periods, &dir)) < 0)
    {
        printf("set %u periods error (%s)\n", periods, snd_strerror (ret));
        return false;
    }
    snd_pcm_uframes_t buffer_size = sample_rate * 0.01;
    if ((ret = snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hw_params,  &buffer_size)) < 0)
    {
        printf("set %lu buffer size error (%s)\n", buffer_size, snd_strerror (ret));
        return false;
    }
    if ((ret = snd_pcm_hw_params(pcm_handle, hw_params)) < 0)
    {
        printf("Cannot set parameters (%s)\n", snd_strerror(ret));
        return false;
    }

    snd_pcm_sw_params_t *sw_params;
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(pcm_handle, sw_params);
    if ((ret = snd_pcm_sw_params(pcm_handle, sw_params)) < 0)
    {
        printf("unable to install sw params (%s)\n", snd_strerror(ret));
        return false;
    }

    setIsOpen(true);
    return true;
}

void AlsaOutput::close()
{
    snd_pcm_close(pcm_handle);
    setIsOpen(false);
}

size_t AlsaOutput::write(void *ptr, size_t size)
{
    snd_pcm_format_t alsa_format = getAlsaFormat(audio_format.getBitsPerSample());
    size_t frame_size = snd_pcm_format_width(alsa_format) / 8 * audio_format.getChannels();
    int num_frames = size / frame_size;
    int ret;
    while ((ret = snd_pcm_writei(pcm_handle, ptr, num_frames)) < 0)
    {
        if (ret == -EAGAIN)
            return 0;
        if(ret == -EPIPE || ret == -ESTRPIPE)
        {
            snd_pcm_status_t *st;
            snd_pcm_status_alloca(&st);
            snd_pcm_status(pcm_handle, st);
            if(snd_pcm_status_get_state(st) == SND_PCM_STATE_XRUN )
            {
                if(snd_pcm_recover(pcm_handle, -EPIPE, 0) < 0)
                {
                    start();
                }
            }
        }
    }
    size_t nbytes = ret * frame_size;
    return nbytes;
}

void AlsaOutput::start()
{
    snd_pcm_prepare(pcm_handle);
}

