#include "EffectApi.h"

void copyToEffect(Effect &effect, const void *data, size_t length)
{
    effect.effect_in_buffer.push(data, length);
}

bool copyFromEffect(Effect &effect, void *data, size_t length)
{
    return effect.effect_out_buffer.pull(data, length);
}
