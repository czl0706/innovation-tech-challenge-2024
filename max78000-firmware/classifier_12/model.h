#ifndef _MODEL_H_
#define _MODEL_H_

// #include <stdio.h>
// #include <stdlib.h>
#include "arm_math.h"
#include "../nn/nn_modules.h"

// 返回值：0 表示成功，-1 表示失败
int init_bw_classifier();
int forward_bw_classifier(const float32_t *input, float32_t *output);
void deinit_bw_classifier();

int forward_bw_classifier_pipeline(const float32_t *input, float32_t *output);

#endif