#ifndef SRC_COMMON_AUDIOFORMAT_H_
#define SRC_COMMON_AUDIOFORMAT_H_


class AudioFormat
{
public:
    AudioFormat(int _sample_rate, int _bits_per_sample, int _channels)
        :sample_rate(_sample_rate), bits_per_sample(_bits_per_sample), channels(_channels){}

    AudioFormat()
        :sample_rate(0), bits_per_sample(0), channels(0){}

    AudioFormat(const AudioFormat &AudioFormatOther)
    {
        sample_rate = AudioFormatOther.sample_rate;
        bits_per_sample = AudioFormatOther.bits_per_sample;
        channels = AudioFormatOther.channels;
    }

    AudioFormat &operator=(const AudioFormat &AudioFormatOther)
    {
        sample_rate = AudioFormatOther.sample_rate;
        bits_per_sample = AudioFormatOther.bits_per_sample;
        channels = AudioFormatOther.channels;
        return *this;
    }

    bool operator==(const AudioFormat other) const
    {
        return sample_rate == other.sample_rate &&
                bits_per_sample == other.bits_per_sample &&
            channels == other.channels;
    }

    bool operator!=(const AudioFormat other) const
    {
        return !(*this == other);
    }

    int getBitsPerSample() const
    {
        return bits_per_sample;
    }

    void setBitsPerSample(int bitsPerSample)
    {
        bits_per_sample = bitsPerSample;
    }

    int getChannels() const
    {
        return channels;
    }

    void setChannels(int channels)
    {
        this->channels = channels;
    }

    int getSampleRate() const
    {
        return sample_rate;
    }

    void setSampleRate(int sampleRate)
    {
        sample_rate = sampleRate;
    }

private:
    int sample_rate;
    int bits_per_sample;
    int channels;
};


#endif /* SRC_COMMON_AUDIOFORMAT_H_ */
