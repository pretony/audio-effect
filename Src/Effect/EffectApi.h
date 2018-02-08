#ifndef SRC_EFFECT_EFFECTAPI_H_
#define SRC_EFFECT_EFFECTAPI_H_

#include "Effect.h"

void copyToEffect(Effect &effect, const void *data, size_t length);
bool copyFromEffect(Effect &effect, void *data, size_t length);

#endif /* SRC_EFFECT_EFFECTAPI_H_ */
