#include "Plugins/SoxEffect.h"
#include "../Effect/EffectApi.h"
#include "../Common/Util.h"
#include <memory>


static std::unique_ptr<EffectPlugin> getEffectPtr(Effect &effect, EffectType effectType)
{
    switch(effectType)
    {
    case EffectType::SOXEFFECT:
        return std::unique_ptr<EffectPlugin>(new SoxEffect(effect));
    default:
        return nullptr;
    }
    return nullptr;
}

void effectThread(Effect &effect, EffectType effectType)
{
    std::unique_ptr<EffectPlugin> effectPlugin = getEffectPtr(effect, effectType);
    if(effectPlugin == nullptr)
    {
        printf("create effect plugin failed, effectType=%d\n", as_integer(effectType));
        return;
    }

    printf("effect plugin start\n");

    //这里要设置主线程的countDown
    effectPlugin->effectAction();
}
