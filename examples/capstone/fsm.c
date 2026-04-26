/**
 * @file  fsm.c
 * @brief Table-driven FSM implementation — Embedded C Capstone
 */

#include "fsm.h"
#include <stdio.h>

/* Transition table: [current_state][event] -> next_state */
static const fsm_state_t fsm_table[STATE_COUNT][EVT_COUNT] = {
    /*               START           DATA_OK         THRESHOLD       ERROR           RESET  */
    [STATE_IDLE]    = { STATE_SENSING, STATE_IDLE,    STATE_IDLE,    STATE_FAULT,    STATE_IDLE    },
    [STATE_SENSING] = { STATE_SENSING, STATE_SENSING, STATE_ALERT,   STATE_FAULT,    STATE_IDLE    },
    [STATE_ALERT]   = { STATE_ALERT,   STATE_SENSING, STATE_ALERT,   STATE_FAULT,    STATE_IDLE    },
    [STATE_FAULT]   = { STATE_FAULT,   STATE_FAULT,   STATE_FAULT,   STATE_FAULT,    STATE_IDLE    },
};

static const char *state_names[STATE_COUNT] = {
    "IDLE", "SENSING", "ALERT", "FAULT"
};

static const char *event_names[EVT_COUNT] = {
    "START", "DATA_OK", "THRESHOLD", "ERROR", "RESET"
};

void fsm_init(fsm_t *fsm) {
    fsm->current = STATE_IDLE;
    fsm->transition_count = 0U;
}

void fsm_dispatch(fsm_t *fsm, fsm_event_t evt) {
    if (fsm->current >= STATE_COUNT || evt >= EVT_COUNT) return;

    fsm_state_t next = fsm_table[fsm->current][evt];

    if (next != fsm->current) {
        printf("  [FSM] %-8s --[%-10s]--> %s\n",
               state_names[fsm->current],
               event_names[evt],
               state_names[next]);
        fsm->current = next;
        fsm->transition_count++;
    }
}

const char *fsm_state_name(fsm_state_t state) {
    if (state < STATE_COUNT) return state_names[state];
    return "UNKNOWN";
}
