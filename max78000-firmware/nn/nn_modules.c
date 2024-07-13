#include "nn_modules.h"

// void conv1d(const float32_t* input, const float32_t* weight, const float32_t* bias, float32_t* output,
//             int in_channels, int out_channels, int input_size, int kernel_size, int stride, int padding) {
//     int output_size = (input_size + 2 * padding - kernel_size) / stride + 1;

//     for (int o = 0; o < out_channels; ++o) {
//         for (int out_idx = 0; out_idx < output_size; ++out_idx) {
//             int in_idx_start = out_idx * stride - padding;
//             int in_idx_end = in_idx_start + kernel_size;
//             int k_start = (in_idx_start < 0) ? -in_idx_start : 0;
//             int k_end = (in_idx_end > input_size) ? kernel_size - (in_idx_end - input_size) : kernel_size;

//             float32_t result = 0.0f;
//             for (int i = 0; i < in_channels; ++i) {
//                 // 使用 arm_dot_prod_f32 來計算點積
//                 float32_t dot_product;
//                 arm_dot_prod_f32(&input[i * input_size + in_idx_start + k_start],
//                                  &weight[o * in_channels * kernel_size + i * kernel_size + k_start],
//                                  k_end - k_start,
//                                  &dot_product);
//                 result += dot_product;
//             }
            
//             // 添加偏置
//             arm_add_f32(&result, &bias[o], &output[o * output_size + out_idx], 1);
//         }
//     }
// }

void conv1d(const float32_t* input, const float32_t* weight, const float32_t* bias, float32_t* output,
            int in_channels, int out_channels, int input_size, int kernel_size, int stride, int padding) {
    int output_size = (input_size + 2 * padding - kernel_size) / stride + 1;

    for (int o = 0; o < out_channels; ++o) {
        for (int out_idx = 0; out_idx < output_size; ++out_idx) {
            int in_idx_start = out_idx * stride - padding;
            int in_idx_end = in_idx_start + kernel_size;
            int k_start = (in_idx_start < 0) ? -in_idx_start : 0;
            int k_end = (in_idx_end > input_size) ? kernel_size - (in_idx_end - input_size) : kernel_size;

            int opt_idx = o * output_size + out_idx;

            output[opt_idx] = bias[o];
            for (int i = 0; i < in_channels; ++i) {
                // 使用 arm_dot_prod_f32 來計算點積
                float32_t dot_product;
                arm_dot_prod_f32(&input[i * input_size + in_idx_start + k_start],
                                 &weight[o * in_channels * kernel_size + i * kernel_size + k_start],
                                 k_end - k_start,
                                 &dot_product);
                output[opt_idx] += dot_product;
            }
        }
    }
}

void relu_inplace(float32_t *input, int size) {
    for (int i = 0; i < size; ++i) {
        if (input[i] < 0) { input[i] = 0; }
    }
}

void linear(const float* input, const float* weight, const float* bias, float* output,
            int input_size, int output_size) {
    for (int o = 0; o < output_size; o++) {
        output[o] = bias[o];
        for (int i = 0; i < input_size; i++) {
            output[o] += input[i] * weight[o * input_size + i];
        }
    }
}
