#include <iostream>
#include <thread>
#include "Input/InputPlugin.h"
#include "Output/OutputPlugin.h"
#include "Effect/EffectPlugin.h"
#include "Effect/Effect.h"

void inputThread(Effect &effect, InputType inputType);
void outputThread(Effect &effect, OutputType outputType);
void effectThread(Effect &effect, EffectType effectType);

int main(int argc, char *argv[])
{
    //创建3个线程，等待所有线程初始化或者打开设备后，再check所有线程初始化的结果，有失败的就关闭所有线程，退出程序。
    Effect effect;
    std::thread input_thread(inputThread, std::ref(effect), InputType::ALSAINPUT);
    std::thread output_thread(outputThread, std::ref(effect), OutputType::ALSAOUTPUT);
    std::thread effect_thread(effectThread, std::ref(effect), EffectType::SOXEFFECT);
    input_thread.join();
    output_thread.join();
    effect_thread.join();
    return 0;
}
