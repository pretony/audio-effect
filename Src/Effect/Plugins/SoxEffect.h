#ifndef SRC_EFFECT_PLUGINS_SOXEFFECT_H_
#define SRC_EFFECT_PLUGINS_SOXEFFECT_H_

#include "../EffectPlugin.h"

class SoxEffect : public EffectPlugin
{
public:
    SoxEffect(Effect &_effect) : EffectPlugin(_effect)
    {
    }
    ~SoxEffect()
    {
    }
    void effectAction();
};

#endif /* SRC_EFFECT_PLUGINS_SOXEFFECT_H_ */
