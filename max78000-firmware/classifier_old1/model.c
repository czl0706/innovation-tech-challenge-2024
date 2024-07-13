#include "model.h"
#include "model_weight.h"

typedef struct {
    float32_t *conv1_output;
    float32_t *conv2_output;
    float32_t *fc_output;
    int conv1_output_size;
    int conv2_output_size;
} BWClassifierOutputs;

typedef struct {
    const float32_t *conv1_weight;
    const float32_t *conv1_bias;
    const float32_t *conv2_weight;
    const float32_t *conv2_bias;
    const float32_t *fc1_weight;
    const float32_t *fc1_bias;
    const float32_t *fc2_weight;
    const float32_t *fc2_bias;
    BWClassifierOutputs outputs;
} BWClassifierModel;

static BWClassifierModel bw_classifier;

void init_bw_classifier() {
    int input_size = 60;  // Assuming input size is 60 based on the flattened size of 360
    bw_classifier.outputs.conv1_output_size = (input_size + 2*2 - 5) / 2 + 1;  // (60 + 4 - 5) / 2 + 1 = 30
    bw_classifier.outputs.conv2_output_size = bw_classifier.outputs.conv1_output_size;  // No change in size for conv2

    bw_classifier.outputs.conv1_output = (float32_t*)malloc(12 * bw_classifier.outputs.conv1_output_size * sizeof(float32_t));
    bw_classifier.outputs.conv2_output = (float32_t*)malloc(12 * bw_classifier.outputs.conv2_output_size * sizeof(float32_t));
    bw_classifier.outputs.fc_output = (float32_t*)malloc(6 * sizeof(float32_t));  // 4 + 2 = 6 outputs

    bw_classifier.conv1_weight = conv1_weight;
    bw_classifier.conv1_bias = conv1_bias;
    bw_classifier.conv2_weight = conv2_weight;
    bw_classifier.conv2_bias = conv2_bias;
    bw_classifier.fc1_weight = fc1_weight;
    bw_classifier.fc1_bias = fc1_bias;
    bw_classifier.fc2_weight = fc2_weight;
    bw_classifier.fc2_bias = fc2_bias;
}

void deinit_bw_classifier() {
    free(bw_classifier.outputs.conv1_output);
    free(bw_classifier.outputs.conv2_output);
    free(bw_classifier.outputs.fc_output);
}

void forward_bw_classifier(const float32_t *input, float32_t *output) {
    // Conv1
    conv1d(input, bw_classifier.conv1_weight, bw_classifier.conv1_bias, bw_classifier.outputs.conv1_output,
           6, 12, 60, 5, 2, 2);
    relu_inplace(bw_classifier.outputs.conv1_output, 12 * bw_classifier.outputs.conv1_output_size);

    // Conv2
    conv1d(bw_classifier.outputs.conv1_output, bw_classifier.conv2_weight, bw_classifier.conv2_bias, bw_classifier.outputs.conv2_output,
           12, 12, bw_classifier.outputs.conv1_output_size, 3, 1, 1);
    relu_inplace(bw_classifier.outputs.conv2_output, 12 * bw_classifier.outputs.conv2_output_size);

    // FC1
    linear(bw_classifier.outputs.conv1_output, bw_classifier.fc1_weight, bw_classifier.fc1_bias, 
           output, 12 * bw_classifier.outputs.conv1_output_size, 4);
    
    // FC2
    linear(bw_classifier.outputs.conv1_output, bw_classifier.fc2_weight, bw_classifier.fc2_bias, 
           output + 4, 12 * bw_classifier.outputs.conv1_output_size, 2);
}