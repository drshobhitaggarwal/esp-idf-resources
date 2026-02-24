#include "student_util.h"

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
