#include "model.h"
#include "model_balance.h"
#include "model_weakpart.h"

typedef struct {
    float32_t *conv1_output;
    float32_t *conv2_output;
    float32_t *fc_output;
    int conv1_output_size;
    int conv2_output_size;
    int fc_output_size;
} ModelOutputs;

typedef struct {
    const float32_t *conv1_weight;
    const float32_t *conv1_bias;
    const float32_t *conv2_weight;
    const float32_t *conv2_bias;
    const float32_t *fc_weight;
    const float32_t *fc_bias;
    ModelOutputs outputs;
} ClassifierModel;

static ClassifierModel balance_classifier;
static ClassifierModel weak_part_classifier;

void init_classifier(ClassifierModel *model, int fc_output_size) {
    int input_size = 60;  // Assuming input size is 60 based on the PyTorch model
    model->outputs.conv1_output_size = (input_size + 2*2 - 5) / 2 + 1;  // (60 + 4 - 5) / 2 + 1 = 30
    model->outputs.conv2_output_size = model->outputs.conv1_output_size;  // No change in size for conv2
    model->outputs.fc_output_size = fc_output_size;

    model->outputs.conv1_output = (float32_t*)malloc(12 * model->outputs.conv1_output_size * sizeof(float32_t));
    model->outputs.conv2_output = (float32_t*)malloc(12 * model->outputs.conv2_output_size * sizeof(float32_t));
    model->outputs.fc_output = (float32_t*)malloc(fc_output_size * sizeof(float32_t));
}

void deinit_classifier(ClassifierModel *model) {
    free(model->outputs.conv1_output);
    free(model->outputs.conv2_output);
    free(model->outputs.fc_output);
}

void forward_classifier(ClassifierModel *model, const float32_t *input) {
    // Conv1
    conv1d(input, model->conv1_weight, model->conv1_bias, model->outputs.conv1_output,
           6, 12, 60, 5, 2, 2);
    relu_inplace(model->outputs.conv1_output, 12 * model->outputs.conv1_output_size);

    // Conv2
    conv1d(model->outputs.conv1_output, model->conv2_weight, model->conv2_bias, model->outputs.conv2_output,
           12, 12, model->outputs.conv1_output_size, 3, 1, 1);
    relu_inplace(model->outputs.conv2_output, 12 * model->outputs.conv2_output_size);

    // FC
    linear(model->outputs.conv2_output, model->fc_weight, model->fc_bias, model->outputs.fc_output,
           12 * model->outputs.conv2_output_size, model->outputs.fc_output_size);
}

// BalanceClassifier
void init_balance_classifier() {
    init_classifier(&balance_classifier, 4);
    balance_classifier.conv1_weight = balance_conv1_weight;
    balance_classifier.conv1_bias = balance_conv1_bias;
    balance_classifier.conv2_weight = balance_conv2_weight;
    balance_classifier.conv2_bias = balance_conv2_bias;
    balance_classifier.fc_weight = balance_fc_1_weight;
    balance_classifier.fc_bias = balance_fc_1_bias;
}

void deinit_balance_classifier() {
    deinit_classifier(&balance_classifier);
}

void forward_balance_classifier(const float32_t *input, float32_t *output) {
    forward_classifier(&balance_classifier, input);
    memcpy(output, balance_classifier.outputs.fc_output, 4 * sizeof(float32_t));
}


// WeakPartClassifier
void init_weak_part_classifier() {
    init_classifier(&weak_part_classifier, 2);
    weak_part_classifier.conv1_weight = weakpart_conv1_weight;
    weak_part_classifier.conv1_bias = weakpart_conv1_bias;
    weak_part_classifier.conv2_weight = weakpart_conv2_weight;
    weak_part_classifier.conv2_bias = weakpart_conv2_bias;
    weak_part_classifier.fc_weight = weakpart_fc_1_weight;
    weak_part_classifier.fc_bias = weakpart_fc_1_bias;
}

void deinit_weak_part_classifier() {
    deinit_classifier(&weak_part_classifier);
}

void forward_weak_part_classifier(const float32_t *input, float32_t *output) {
    forward_classifier(&weak_part_classifier, input);
    memcpy(output, weak_part_classifier.outputs.fc_output, 2 * sizeof(float32_t));
}