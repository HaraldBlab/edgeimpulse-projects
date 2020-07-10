#include "mbed.h"
#include "ei_run_classifier.h"
#include "numpy.hpp"

static const float features[] = {
    // copy raw features here (for example from the 'Live classification' page)
    // see https://docs.edgeimpulse.com/docs/running-your-impulse-mbed
    -1.0600, 9.9200, -0.9900, -1.0600, 9.9200, -0.9900, -1.0900, 9.9300, -0.9500, -1.0900, 9.9100, -0.9900, -1.0900, 9.7900, -1.1600, -1.1000, 9.7600, -1.2200, -1.1500, 9.7200, -1.2700, -1.0500, 9.7200, -1.3000, -1.0500, 9.7200, -1.3000, -1.1100, 9.7000, -1.2300, -1.2000, 9.7200, -1.2100, -1.1200, 9.8100, -1.1200, -1.0700, 9.8200, -1.1600, -1.1700, 9.7500, -1.1700, -1.1200, 9.7700, -1.2000, -1.1200, 9.7700, -1.2000, -1.1300, 9.7300, -1.2300, -1.1500, 9.7500, -1.1800, -1.1300, 9.7400, -1.2300, -1.1900, 9.6100, -1.2800, -1.1900, 9.6500, -1.2900, -1.3300, 9.7000, -1.2500, -1.3300, 9.7000, -1.2500, -1.2600, 9.8300, -1.1100, -1.0900, 9.9200, -1.1500, -1.1300, 9.8900, -1.1800, -1.2300, 9.9400, -1.1500, -1.2900, 9.8600, -1.2200, -1.3000, 9.8800, -1.2200, -1.3900, 9.8600, -1.1900, -1.3900, 9.8600, -1.1900, -1.3500, 9.8000, -1.2200, -1.0900, 9.9200, -1.2300, -0.9600, 9.9900, -1.2700, -0.9200, 9.8200, -1.4600, -1.1500, 9.4400, -1.8900, -0.9500, 9.8600, -1.1600, -0.9500, 9.8600, -1.1600, -0.5800, 10.0900, -1.2000, -0.3600, 10.0100, -1.4500, -1.2500, 9.0900, -1.8600, -1.5600, 8.2900, -2.0900, -3.1300, 7.4000, -1.7600, -5.3600, 5.9800, -1.6900, -5.3600, 5.9800, -1.6900, -7.2000, 4.6800, -1.8900, -6.6500, 3.3400, -2.6100, -7.6300, 2.7700, -2.5000, -8.1000, 2.7300, -2.4400, -8.3800, 1.9400, -2.1600, -10.6100, -0.6500, -2.3300, -10.6100, -0.6500, -2.3300, -13.9600, -2.8900, -1.3300, -15.2000, -1.9100, -1.3100, -13.9900, -1.5600, -2.0000, -13.7400, -1.5000, -2.6000, -14.2300, -1.7100, -2.3600, -14.8500, -1.2000, -0.5100, -13.4900, 0.1200, -0.0400, -13.4900, 0.1200, -0.0400, -11.9100, 0.5700, -0.5700, -10.6800, 1.9200, -1.2700, -10.1200, 2.2900, -2.2700, -8.7700, 2.3300, -2.7400, -6.2800, 2.3300, -2.2500, -3.3800, 2.9700, -1.8800, -3.3800, 2.9700, -1.8800, -0.9300, 3.5600, -1.1200, 0.8800, 6.0600, 0.4000, 1.8200, 7.2200, -0.3100, 2.2000, 6.6900, -0.8200, 3.0800, 7.1300, -1.0600, 5.0000, 6.9700, -0.4900, 5.0000, 6.9700, -0.4900, 5.9900, 5.8000, -1.7500, 7.0400, 4.9100, -1.7300, 8.3100, 5.2500, -1.5600, 11.8600, 5.0000, -1.8500, 10.8800, 5.7100, -2.2500, 10.5600, 4.9300, -2.1100, 10.5600, 4.9300, -2.1100, 11.1300, 5.1000, -2.9800, 11.3400, 6.7800, -2.4600, 11.0200, 6.7800, -2.6300, 10.5500, 6.8400, -2.6100, 10.8400, 6.8300, -0.7000, 10.0400, 7.0300, -2.6300, 11.0500, 6.6200, -1.7400, 11.0500, 6.6200, -1.7400, 10.5500, 6.7100, -2.1800, 10.1500, 6.1900, -2.5100, 9.1000, 7.3900, -1.8700, 8.2800, 7.5500, -2.1700, 6.2800, 7.2000, -2.0800, 3.3100, 6.9500, -1.5800, 3.3100, 6.9500, -1.5800, 0.3800, 7.0000, -0.7100, -1.2700, 7.7700, 0.7400, -2.7400, 7.3400, 1.1000, -4.6300, 6.2700, -0.3100, -7.0900, 5.6400, 0.2900, -7.3500, 5.5800, 0.2800, -7.3500, 5.5800, 0.2800, -6.4300, 5.4700, -0.8900, -5.7400, 4.7900, -1.1200, -6.9300, 3.2300, -1.6400, -9.5300, 2.1300, -0.2600, -11.3000, 0.5500, 0.9200, -12.4400, 0.7600, 0.5400, -11.7200, 1.0200, 0.1100, -11.7200, 1.0200, 0.1100, -11.6800, 1.1500, -0.3900, -11.4300, 0.4000, -0.7000, -12.2000, -0.0900, -1.0100, -12.4500, 0.5900, -0.4300, -11.9900, 1.3500, 0.1200, -12.2400, 1.2700, -0.5100, -12.2400, 1.2700, -0.5100, -11.7300, 1.9600, -0.2200, -11.1900, 2.0200, -0.3200, -10.8200, 2.6900, -0.4400, -10.4300, 2.5900, -1.4100, -9.4500, 2.4600, -2.4400, -6.5500, 3.5300, -2.1400, -6.5500, 3.5300, -2.1400
};

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void print_predictions(const ei_impulse_result_t result)
{
    printf("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        printf("%s: ", result.classification[ix].label);
        printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        printf(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            printf(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    printf("%.3f", result.anomaly);
#endif
    printf("]\n");
}

int get_best_classification(const ei_impulse_result_t &result)
{
    int bestIndex = -1;
    float bestValue = 0.f;
    const char *bestLabel = nullptr;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > bestValue)
        {
            bestIndex = ix;
        }   
    }
    return bestIndex;
}
int main() {
    printf("Edge Impulse standalone inferencing (Mbed)\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    ei_impulse_result_t result = { 0 };

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, true);
    printf("run_classifier returned: %d\n", res);

    if (res != 0) return 1;

    printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);

    // print the predictions
    print_predictions(result);

    // output the best fit
    int bestIndex = get_best_classification(result);
    const char *bestLabel = bestIndex == -1 ? nullptr : result.classification[bestIndex].label;

    printf("detected: '%s'", bestLabel == nullptr ? "undefined" : bestLabel);
    printf("\ndone.\n");
}
