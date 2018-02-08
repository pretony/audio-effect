#ifndef SRC_EFFECT_EFFECTPLUGIN_H_
#define SRC_EFFECT_EFFECTPLUGIN_H_

#include "Effect.h"
#include "../Common/AudioFormat.h"

enum class EffectType
{
    UNDEFINED = 0,
    SOXEFFECT,
};

class EffectPlugin
{
public:
    EffectPlugin(Effect &_effect):effect(_effect)
    {
    }
    virtual ~EffectPlugin()
    {
    }
    virtual void effectAction() = 0;
    Effect &effect;
    AudioFormat input_format;
    AudioFormat output_format;
};

#endif /* SRC_EFFECT_EFFECTPLUGIN_H_ */
