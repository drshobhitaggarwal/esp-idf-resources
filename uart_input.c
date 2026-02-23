#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For INT_MAX and INT_MIN
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define types for the parser result
typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} data_type_t;

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
void app_main(void) {
    // Variables for storage
    int user_age = 0;
    float user_pi = 0.0f;
    char user_name[32] = {0};

    while (1) {
        // --- SCENARIO 1: We ONLY want an Integer ---
        // We pass NULL for float and string because we don't need them here.
        printf("\n[Step 1: Calibration]");
        data_type_t result1 = get_user_input("Enter your age (int): ", &user_age, NULL, NULL, 0);
        
        if (result1 == TYPE_INT) {
            printf("Confirmed. You are %d years old.\n", user_age);
        } else {
            printf("Warning: That was not a valid age!\n");
        }

        // --- SCENARIO 2: We ONLY want a Float ---
        printf("\n[Step 2: Math Test]");
        data_type_t result2 = get_user_input("Enter the value of Pi (float): ", NULL, &user_pi, NULL, 0);
        
        if (result2 == TYPE_FLOAT) {
            printf("Confirmed. Pi is approximately %.4f\n", user_pi);
        } else {
            printf("Warning: You didn't enter a decimal number.\n");
        }

        // --- SCENARIO 3: We accept ANYTHING (Catch-all) ---
        printf("\n[Step 3: Identification]");
        data_type_t result3 = get_user_input("Enter a username (string): ", NULL, NULL, user_name, sizeof(user_name));
        
        if (result3 != TYPE_NONE) {
            printf("Welcome, %s!\n", user_name);
        }

        printf("\n--- Setup Complete. Restarting in 5 seconds... ---\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
