#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "../nn/nn_modules.h"

void init_bw_classifier();
void deinit_bw_classifier();
void forward_bw_classifier(const float32_t *input, float32_t *output);

#endif