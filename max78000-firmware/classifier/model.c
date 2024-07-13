#include <stdlib.h>
#include "arm_math.h"
#include "model.h"
#include "weights.h"

// 定义模型结构
typedef struct {
    // 指向权重和偏置的指针
    const float32_t *conv1_weight;
    const float32_t *conv1_bias;
    const float32_t *conv2_weight;
    const float32_t *conv2_bias;
    const float32_t *fc1_weight;
    const float32_t *fc1_bias;
    const float32_t *fc2_weight;
    const float32_t *fc2_bias;

    // 中间结果的缓冲区
    float32_t *conv1_output;
    float32_t *conv2_output;

    // 输入和输出大小
    int input_size;
    int conv1_output_size;
    int conv2_output_size;

    // 初始化标志
    int is_initialized;
} BWClassifier;

static BWClassifier classifier = {0};

// 初始化函数
int init_bw_classifier() {
    if (classifier.is_initialized) {
        return 0;  // 已经初始化过
    }

    int input_size = 60;

    classifier.input_size = input_size;
    classifier.conv1_output_size = (input_size + 2*2 - 5) / 2 + 1;
    classifier.conv2_output_size = classifier.conv1_output_size;

    classifier.conv1_output = (float32_t*)malloc(4 * classifier.conv1_output_size * sizeof(float32_t));
    classifier.conv2_output = (float32_t*)malloc(6 * classifier.conv2_output_size * sizeof(float32_t));

    if (classifier.conv1_output == NULL || classifier.conv2_output == NULL) {
        free(classifier.conv1_output);
        free(classifier.conv2_output);
        return -1;  // 内存分配失败
    }

    classifier.conv1_weight = conv1_weight;
    classifier.conv1_bias = conv1_bias;
    classifier.conv2_weight = conv2_weight;
    classifier.conv2_bias = conv2_bias;
    classifier.fc1_weight = fc1_weight;
    classifier.fc1_bias = fc1_bias;
    classifier.fc2_weight = fc2_weight;
    classifier.fc2_bias = fc2_bias;

    classifier.is_initialized = 1;
    return 0;
}

// 反初始化函数
void deinit_bw_classifier() {
    if (classifier.is_initialized) {
        free(classifier.conv1_output);
        free(classifier.conv2_output);
        classifier.is_initialized = 0;
    }
}

// 前向传播函数
int forward_bw_classifier(const float32_t *input, float32_t *output) {
    if (!classifier.is_initialized) {
        return -1;  // 模型未初始化
    }

    // Conv1 + ReLU
    conv1d(input, classifier.conv1_weight, classifier.conv1_bias, classifier.conv1_output,
           6, 4, classifier.input_size, 5, 2, 2);
    relu_inplace(classifier.conv1_output, 4 * classifier.conv1_output_size);

    // Conv2 + ReLU
    conv1d(classifier.conv1_output, classifier.conv2_weight, classifier.conv2_bias, classifier.conv2_output,
           4, 6, classifier.conv1_output_size, 3, 1, 1);
    relu_inplace(classifier.conv2_output, 6 * classifier.conv2_output_size);

    // FC1
    linear(classifier.conv2_output, classifier.fc1_weight, classifier.fc1_bias, output, 180, 4);

    // FC2
    linear(classifier.conv2_output, classifier.fc2_weight, classifier.fc2_bias, output + 4, 180, 2);

    return 0;
}

int forward_bw_classifier_pipeline(const float32_t *input, float32_t *output) {
    if (!classifier.is_initialized) {
        return -1;  // 模型未初始化
    }

    static int stage = 0;

    if (stage == 3) {
        stage = 0;
    }

    if (stage == 0 && input == NULL) {
        return -1;  // 输入为空
    }

    switch (stage)
    {
    case 0: {
        // Conv1 + ReLU
        conv1d(input, classifier.conv1_weight, classifier.conv1_bias, classifier.conv1_output,
           6, 4, classifier.input_size, 5, 2, 2);
        relu_inplace(classifier.conv1_output, 4 * classifier.conv1_output_size);
    }
        break;
    case 1: {
        // Conv2 + ReLU
        conv1d(classifier.conv1_output, classifier.conv2_weight, classifier.conv2_bias, classifier.conv2_output,
            4, 6, classifier.conv1_output_size, 3, 1, 1);
        relu_inplace(classifier.conv2_output, 6 * classifier.conv2_output_size);
    }
        break;
    case 2: {
        // FC Layer
        linear(classifier.conv2_output, classifier.fc1_weight, classifier.fc1_bias, output, 180, 4);
        linear(classifier.conv2_output, classifier.fc2_weight, classifier.fc2_bias, output + 4, 180, 2);
    }
        break;
    
    default:
        break;
    }

    return stage++;

    // // Conv1 + ReLU
    // conv1d(input, classifier.conv1_weight, classifier.conv1_bias, classifier.conv1_output,
    //        6, 4, classifier.input_size, 5, 2, 2);
    // relu_inplace(classifier.conv1_output, 4 * classifier.conv1_output_size);

    // // Conv2 + ReLU
    // conv1d(classifier.conv1_output, classifier.conv2_weight, classifier.conv2_bias, classifier.conv2_output,
    //        4, 6, classifier.conv1_output_size, 3, 1, 1);
    // relu_inplace(classifier.conv2_output, 6 * classifier.conv2_output_size);

    // // FC1
    // linear(classifier.conv2_output, classifier.fc1_weight, classifier.fc1_bias, output, 180, 4);

    // // FC2
    // linear(classifier.conv2_output, classifier.fc2_weight, classifier.fc2_bias, output + 4, 180, 2);

    // return 0;
}