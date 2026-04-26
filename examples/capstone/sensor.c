/**
 * @file  sensor.c
 * @brief Sensor simulation implementation — Embedded C Capstone
 */

#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   /* rand() for simulation */

bool sensor_init(sensor_t *s, uint8_t id, const char *name,
                 float lo, float hi) {
    if (s == NULL || name == NULL) return false;
    memset(s, 0, sizeof(*s));
    s->id = id;
    snprintf(s->name, SENSOR_NAME_LEN, "%s", name);
    s->min_threshold = lo;
    s->max_threshold = hi;
    s->status = SENSOR_OK;
    return true;
}

bool sensor_read(sensor_t *s) {
    if (s == NULL) return false;

    /* Simulate ADC reading: base value + small random noise */
    float base  = 22.0f + (float)(s->id * 3);
    float noise = ((float)(rand() % 100) - 50.0f) / 100.0f * 5.0f;
    s->value = base + noise;

    /* Validate range */
    if (s->value < s->min_threshold || s->value > s->max_threshold) {
        s->status = SENSOR_ERROR;
        return false;
    }

    s->status = SENSOR_OK;
    return true;
}

void sensor_print(const sensor_t *s) {
    if (s == NULL) return;
    const char *status_str[] = { "OK", "TIMEOUT", "ERROR", "OFFLINE" };
    printf("  [Sensor %u | %-12s] Value: %6.2f | Status: %s\n",
           s->id, s->name, s->value,
           (s->status < 4U) ? status_str[s->status] : "???");
}
