#include "SoxEffect.h"
#include "../../Common/sox.h"
#include <stdio.h>
#include <stdlib.h>

void SoxEffect::effectAction()
{
    sox_format_t * in, * out;
    sox_effects_chain_t * chain;
    sox_effect_t * e;
    char * args[10];
    sox_init();
    sox_signalinfo_t interm_signal;

    sox_signalinfo_t in_signal;
    in_signal.rate = (sox_rate_t)(input_format.getSampleRate());
    in_signal.precision = input_format.getBitsPerSample();
    in_signal.channels = input_format.getChannels();
    in_signal.length = 0;
    in_signal.mult = NULL;

    sox_encodinginfo_t in_encoding;
    in_encoding.bits_per_sample = in_signal.precision;
    in_encoding.encoding = SOX_ENCODING_SIGN2;

    sox_signalinfo_t out_signal;
    out_signal.rate = (sox_rate_t)(output_format.getSampleRate());
    out_signal.precision = output_format.getBitsPerSample();
    out_signal.channels = output_format.getChannels();
    out_signal.length = 0;
    out_signal.mult = NULL;

    int cmd = 0;
    int *cmdInt = (int *)(&cmd);
    int input_end = 0;

    in = sox_open_read_stream(effect.effect_in_buffer.get_rb(), effect.effect_in_buffer.get_mutex(), effect.effect_in_buffer.get_cond_notFull(), effect.effect_in_buffer.get_cond_notEmpty(), &input_end, cmdInt, DSDTYPE_UNKNOWN, &in_signal, &in_encoding, "raw");

    out = sox_open_write_stream(effect.effect_out_buffer.get_rb(), effect.effect_out_buffer.get_mutex(), effect.effect_out_buffer.get_cond_notFull(), effect.effect_out_buffer.get_cond_notEmpty(), cmdInt, DSDTYPE_UNKNOWN, &out_signal, NULL, "raw");

    chain = sox_create_effects_chain(&in->encoding, &out->encoding);
    interm_signal = in->signal;

    e = sox_create_effect(sox_find_effect("input"));
    args[0] = (char *)in, sox_effect_options(e, 1, args);
    sox_add_effect(chain, e, &interm_signal, &in->signal);
    free(e);

    e = sox_create_effect(sox_find_effect("rate"));
    int out_rate_int = (int)out_signal.rate;
    char out_rate[10] = {0};
    snprintf(out_rate, 10, "%d", out_rate_int);

    args[0] = (char *)"-v";
    args[1] = out_rate;
    sox_effect_options(e, 2, args);

    sox_add_effect(chain, e, &interm_signal, &out->signal);
    free(e);

    e = sox_create_effect(sox_find_effect("output"));
    args[0] = (char *)out, sox_effect_options(e, 1, args);
    sox_add_effect(chain, e, &interm_signal, &out->signal);
    free(e);

    sox_flow_effects(chain, NULL, NULL);
    sox_delete_effects_chain(chain);
    sox_close(out);
    sox_close(in);
    sox_quit();
}
