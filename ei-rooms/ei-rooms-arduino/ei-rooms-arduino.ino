/* Edge Impulse Arduino examples
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Includes ---------------------------------------------------------------- */
#include <rooms_inference.h>

// the type used for features inpt
typedef float featuresType[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
/**
 * @brief      Type sued to copy raw feature data in out_ptr
 * 
 *             Function called by inference library
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */
typedef int (*features_get_data )(size_t, size_t, float *);

// raw features from the 'Live classification' page
// see https://docs.edgeimpulse.com/docs/running-your-impulse-arduino
static const featuresType cookingFeatures = {
    56.0000, 14.7000, 55.0000, 15.4000, 54.0000, 14.9000, 54.0000, 14.7000, 54.0000, 15.1000, 54.0000, 15.2000
};
int cooking_raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, cookingFeatures + offset, length * sizeof(float));
    return 0;
}
static const featuresType livingFeatures = {
    44.0000, 17.7000, 45.0000, 17.7000, 44.0000, 17.6000, 44.0000, 17.7000, 44.0000, 17.3000, 44.0000, 17.5000
};
int living_raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, livingFeatures + offset, length * sizeof(float));
    return 0;
}

static const featuresType sleepingFeatures = {
    74.0000, 12.1000, 74.0000, 12.2000, 74.0000, 12.1000, 74.0000, 12.1000, 74.0000, 12.2000, 74.0000, 12.1000
};
int sleeping_raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, sleepingFeatures + offset, length * sizeof(float));
    return 0;
}

/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    Serial.println("Edge Impulse Inferencing Demo");
}

void print_predictions(const ei_impulse_result_t result)
{
    ei_printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%s: ", result.classification[ix].label);
        ei_printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            ei_printf(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("%.3f", result.anomaly);
#endif
    ei_printf("]\n");
}

int get_best_classification(const ei_impulse_result_t &result)
{
    int bestIndex = -1;
    float bestValue = 0.8f; // TODO: get the value from the model
    const char *bestLabel = nullptr;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > bestValue)
        {
            bestIndex = ix;
            bestValue = result.classification[bestIndex].value;
        }   
    }
    return bestIndex;
}

int run_classification(const featuresType &features, const features_get_data raw_feature_get_data, bool debug) {
    ei_printf("Running classification of feature\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    ei_impulse_result_t result = { 0 };

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, debug);
    if (debug)
        ei_printf("run_classifier returned: %d\n", res);

    if (res != 0) return 1;

    if (debug)
        ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
            result.timing.dsp, result.timing.classification, result.timing.anomaly);

    // print the predictions
    if (debug)
        print_predictions(result);

    // output the best fit
    int bestIndex = get_best_classification(result);
    const char *bestLabel = bestIndex == -1 ? nullptr : result.classification[bestIndex].label;

    ei_printf("detected: '%s'", bestLabel == nullptr ? "uncertain" : bestLabel);
    ei_printf("\ndone.\n");
    
    return 0;
}

/**
 * @brief      Arduino main function
 */
void loop()
{
    ei_printf("Edge Impulse standalone inferencing (Arduino)\n");

    bool debug = false;
    
    int ret = run_classification(cookingFeatures, cooking_raw_feature_get_data, debug);
    ret = run_classification(livingFeatures, living_raw_feature_get_data, debug);
    // TODO: get "uncertain" here, in LiveClassification it is detected properly
    ret = run_classification(sleepingFeatures, sleeping_raw_feature_get_data, debug);

    delay(1000);
}

/**
 * @brief      Printf function uses vsnprintf and output using Arduino Serial
 *
 * @param[in]  format     Variable argument list
 */
void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        Serial.write(print_buf);
    }
}
