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

/*
 * Board: LOLIN(WEMOS) D1 R2 & mini
 * CPU Frequence: 160 MHz
 *
 * FIX: library.properties
 * #depends=Arduino_LSM9DS1, PDM, Arduino_OV767X
 */
/* Includes ---------------------------------------------------------------- */
#include <flames_inferencing.h>

#include "DataCollector.h"

DataCollector collector;

#include "DataTelegramBOT.h"
DataTelegramBOT telegramBOT;

/* data analyzer */
unsigned long requestLoading = 60000;  /* time span for sending "put some wood in the oven"  */
unsigned long lastTimeLoadingSent;
int lastIndex = -1; /* previous analyzing state */

static float features[] = {
    // copy raw features here (for example from the 'Live classification' page)
    // see https://docs.edgeimpulse.com/docs/running-your-impulse-arduino
    161, 47, 25, 161, 47, 25, 161, 47, 25, 159, 47, 25, 151, 47, 25, 160, 47, 25
};

/**
 * @brief      Copy raw feature data in out_ptr
 *             Function called by inference library
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
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

    collector.setup();

    telegramBOT.setup();
}

/**
 * @brief      Arduino main function
 */
void loop()
{
    ei_printf("Edge Impulse standalone inferencing (Arduino)\n");

    // read data from the sensor
    collector.collect(&features[0], EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    
    // print the features to classify
    ei_printf("Features (");
    for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++)
      ei_printf("%.2f ", features[i]);
    ei_printf(")\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %lu items, but had %lu\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        delay(1000);
        return;
    }

    ei_impulse_result_t result = { 0 };

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = &raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
    ei_printf("run_classifier returned: %d\n", res);

    if (res != 0) return;

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    ei_printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
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

    // human-readable predictions
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

    /* update the state visible */
    float bestValue = 0.0;
    int bestIndex = -1;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
      if (result.classification[ix].value > bestValue) {
        bestIndex = ix;
        bestValue = result.classification[bestIndex].value;
      }
    }
    if (bestIndex != -1) {
      String state(result.classification[bestIndex].label);
      telegramBOT.state = state;
    }
    /* FIX:
     * model is not accurate on getting the loading state
     * so I add a simple treshhold handling here
     * Btw this means that I don't need ML here hmmm
     */
    #define MODEL_LOADING 3
    const float flameValueLoading = 678.0;
    float flameValue = 0.0;
    for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i+=3)
      flameValue += features[i];
    flameValue /= 3.0;
    if (flameValue > flameValueLoading) {
      bestIndex = MODEL_LOADING;
      
      String state(result.classification[bestIndex].label);
      telegramBOT.state = state;
    }

    /* send alert message if "loading" for too long 
     * do it after the loop to not run into race condition
     */
    if (bestIndex == MODEL_LOADING) {
        if (lastIndex == MODEL_LOADING) {
          if (millis() > lastTimeLoadingSent + requestLoading) {
            ei_printf("%s\n", "Put more wood in the oven");
            telegramBOT.sendMessage("Put more wood in the oven");
            lastTimeLoadingSent = millis();
          }
        }
    }
    lastIndex = bestIndex;

    /* communicate with the telegramBOT */
    telegramBOT.loop();

    /* wait for next event to detect */
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