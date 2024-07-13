#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "../nn/nn_modules.h"

void init_balance_classifier();
void deinit_balance_classifier();
void forward_balance_classifier(const float32_t *input, float32_t *output);
void init_weak_part_classifier();
void deinit_weak_part_classifier();
void forward_weak_part_classifier(const float32_t *input, float32_t *output);

#endif