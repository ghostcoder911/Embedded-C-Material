#ifndef SENSOR_H
#define SENSOR_H

/**
 * @file  sensor.h
 * @brief Sensor abstraction layer — Embedded C Capstone
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_SENSORS     4U
#define SENSOR_NAME_LEN 16U

typedef enum {
    SENSOR_OK      = 0U,
    SENSOR_TIMEOUT = 1U,
    SENSOR_ERROR   = 2U,
    SENSOR_OFFLINE = 3U
} sensor_status_t;

typedef struct {
    uint8_t  id;
    char     name[SENSOR_NAME_LEN];
    float    value;
    float    min_threshold;
    float    max_threshold;
    sensor_status_t status;
} sensor_t;

/* Sensor HAL — simulated for desktop testing */
bool sensor_init(sensor_t *s, uint8_t id, const char *name,
                 float lo, float hi);
bool sensor_read(sensor_t *s);
void sensor_print(const sensor_t *s);

#endif /* SENSOR_H */
