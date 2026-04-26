/**
 * @file  main.c
 * @brief Capstone: Multi-Sensor Pipeline with FSM — Embedded C Notes
 *
 * Demonstrates:
 *  - Struct-based sensor abstraction (HAL pattern)
 *  - Table-driven Finite State Machine
 *  - Function pointers & callbacks
 *  - Defensive programming & error handling
 *  - Ring buffer pattern
 *
 * Build: make  (or: gcc -std=c11 -Wall -Wextra -o sensor_fsm main.c sensor.c fsm.c)
 * Run:   ./sensor_fsm
 */

#include "sensor.h"
#include "fsm.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ── Configuration ──────────────────────────────────────────────── */
#define NUM_SENSORS      3U
#define PIPELINE_CYCLES  8U

/* ── Alert callback ─────────────────────────────────────────────── */
typedef void (*alert_fn_t)(uint8_t sensor_id, float value, const char *msg);

static void default_alert(uint8_t id, float val, const char *msg) {
    printf("  [ALERT] Sensor %u | %.2f | %s\n", id, val, msg);
}

/* ── System context ─────────────────────────────────────────────── */
typedef struct {
    sensor_t    sensors[NUM_SENSORS];
    fsm_t       fsm;
    alert_fn_t  on_alert;
    uint32_t    cycle;
    uint32_t    alert_count;
} system_ctx_t;

/* ── Main pipeline cycle ─────────────────────────────────────────── */
static void pipeline_cycle(system_ctx_t *ctx) {
    printf("\n--- Cycle %u | State: %s ---\n",
           ctx->cycle, fsm_state_name(ctx->fsm.current));

    uint8_t fault_count = 0U;
    uint8_t alert_count = 0U;

    for (uint8_t i = 0U; i < NUM_SENSORS; i++) {
        sensor_t *s = &ctx->sensors[i];
        bool ok = sensor_read(s);
        sensor_print(s);

        if (!ok) {
            fault_count++;
            if (ctx->on_alert) {
                ctx->on_alert(s->id, s->value, "Out-of-range");
                ctx->alert_count++;
            }
        } else if (s->value > (s->max_threshold - 2.0f)) {
            /* Near upper threshold — alert */
            alert_count++;
            if (ctx->on_alert) {
                ctx->on_alert(s->id, s->value, "Near upper threshold");
                ctx->alert_count++;
            }
        }
    }

    /* FSM event dispatch based on readings */
    if (fault_count > 0U) {
        fsm_dispatch(&ctx->fsm, EVT_ERROR);
    } else if (alert_count > 0U) {
        fsm_dispatch(&ctx->fsm, EVT_THRESHOLD);
    } else {
        fsm_dispatch(&ctx->fsm, EVT_DATA_OK);
    }

    ctx->cycle++;
}

/* ── Entry point ─────────────────────────────────────────────────── */
int main(void) {
    srand((unsigned int)time(NULL));

    printf("========================================\n");
    printf("  Embedded C Capstone: Sensor Pipeline  \n");
    printf("========================================\n");

    system_ctx_t sys;
    memset(&sys, 0, sizeof(sys));
    sys.on_alert = default_alert;

    /* Initialize sensors with name, low threshold, high threshold */
    sensor_init(&sys.sensors[0], 0U, "TEMP_BOARD", 10.0f, 35.0f);
    sensor_init(&sys.sensors[1], 1U, "TEMP_CPU",   15.0f, 40.0f);
    sensor_init(&sys.sensors[2], 2U, "VOLTAGE",    18.0f, 30.0f);

    /* Initialize FSM and start it */
    fsm_init(&sys.fsm);
    fsm_dispatch(&sys.fsm, EVT_START);

    /* Run the pipeline for N cycles */
    for (uint32_t i = 0U; i < PIPELINE_CYCLES; i++) {
        pipeline_cycle(&sys);

        /* Simulate a fault recovery after cycle 5 */
        if (i == 5U && sys.fsm.current == STATE_FAULT) {
            printf("  [SYS] Attempting fault recovery...\n");
            fsm_dispatch(&sys.fsm, EVT_RESET);
            fsm_dispatch(&sys.fsm, EVT_START);
        }
    }

    printf("\n========================================\n");
    printf("  Pipeline complete.\n");
    printf("  Cycles run:     %u\n", sys.cycle);
    printf("  FSM transitions: %u\n", sys.fsm.transition_count);
    printf("  Alerts fired:   %u\n", sys.alert_count);
    printf("  Final state:    %s\n", fsm_state_name(sys.fsm.current));
    printf("========================================\n");

    return 0;
}
