#ifndef FSM_H
#define FSM_H

/**
 * @file  fsm.h
 * @brief Table-driven Finite State Machine — Embedded C Capstone
 */

#include <stdint.h>

typedef enum {
    STATE_IDLE    = 0U,
    STATE_SENSING = 1U,
    STATE_ALERT   = 2U,
    STATE_FAULT   = 3U,
    STATE_COUNT
} fsm_state_t;

typedef enum {
    EVT_START     = 0U,
    EVT_DATA_OK   = 1U,
    EVT_THRESHOLD = 2U,
    EVT_ERROR     = 3U,
    EVT_RESET     = 4U,
    EVT_COUNT
} fsm_event_t;

typedef struct {
    fsm_state_t current;
    uint32_t    transition_count;
} fsm_t;

void        fsm_init(fsm_t *fsm);
void        fsm_dispatch(fsm_t *fsm, fsm_event_t evt);
const char *fsm_state_name(fsm_state_t state);

#endif /* FSM_H */
