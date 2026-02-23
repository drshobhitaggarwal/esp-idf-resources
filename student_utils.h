#ifndef STUDENT_UTILS_H
#define STUDENT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// --- TYPES ---
typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} data_type_t;

// --- INPUT FUNCTIONS ---

/**
 * @brief Professional-grade input parser. 
 * Prints prompt once and waits for user input. Supports NULL for ignored types.
 */
data_type_t get_user_input(const char* prompt, int* i_out, float* f_out, char* s_out, size_t s_max) {
    char buffer[128];
    char *endptr;
    int index = 0;

    printf("\n%s", prompt);
    fflush(stdout);

    // Manually build the string by waiting for characters
    while (index < sizeof(buffer) - 1) {
        int c = getchar();

        // If no character is available (non-blocking), wait a bit
        if (c == EOF || c == 0xFF) {
            vTaskDelay(pdMS_TO_TICKS(10)); 
            continue;
        }

        // Echo the character back to the user
        putchar(c);
        fflush(stdout);

        // Handle Newline/Enter
        if (c == '\n' || c == '\r') {
            buffer[index] = '\0';
            printf("\n");
            break;
        }
        
        // Handle Backspace
        if (c == '\b' && index > 0) {
            index--;
            printf(" \b"); // Clear char on screen
            continue;
        }

        buffer[index++] = (char)c;
    }

    if (strlen(buffer) == 0) return TYPE_NONE;

    // --- Your original parsing logic starts here ---
    // 1. Try Integer
    long l_val = strtol(buffer, &endptr, 10);
    if (*endptr == '\0') {
        if (i_out != NULL) *i_out = (int)l_val; 
        return TYPE_INT;
    }

    // 2. Try Float
    float f_val = strtof(buffer, &endptr);
    if (*endptr == '\0') {
        if (f_out != NULL) *f_out = f_val;
        return TYPE_FLOAT;
    }

    // 3. Try String
    if (s_out != NULL && s_max > 0) {
        strncpy(s_out, buffer, s_max - 1);
        s_out[s_max - 1] = '\0';
    }
    return TYPE_STRING;
}
// --- MATH & LOGIC HELPERS ---

/**
 * @brief Re-maps a value from one range to another.
 * Example: Converting a sensor reading (0-4095) to percentage (0-100).
 */
static inline float map_value(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Ensures a value stays within the min and max boundaries.
 */
static inline float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// --- TIMING HELPERS ---

/**
 * @brief Non-blocking timer check. 
 * Returns true if the specified interval in milliseconds has passed.
 */
static inline bool is_timeout(TickType_t last_tick, uint32_t interval_ms) {
    return (xTaskGetTickCount() - last_tick) >= pdMS_TO_TICKS(interval_ms);
}

#endif // STUDENT_UTILS_H
