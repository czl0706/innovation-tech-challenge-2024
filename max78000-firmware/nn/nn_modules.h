#ifndef _NN_MODULES_H_
#define _NN_MODULES_H_

#include <string.h>
#include <math.h>
#include "arm_math.h"
#include "arm_const_structs.h"

void conv1d(const float32_t* input, const float32_t* weight, const float32_t* bias, float32_t* output,
            int in_channels, int out_channels, int input_size, int kernel_size, int stride, int padding);
void relu_inplace(float32_t *input, int size);
void linear(const float32_t* input, const float32_t* weight, const float32_t* bias, float32_t* output,
            int input_size, int output_size);
            
#endif