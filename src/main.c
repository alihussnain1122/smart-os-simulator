#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "integrated.h"
#include "individual.h"

// Colors
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

// Clear screen
void clear_screen() {
    system("clear || cls"); // Works on Linux and Windows
}

// Boot screen with animation
void boot_screen() {
    clear_screen();
    printf(BLUE);
    printf("\n=========================================\n");
    printf("|        SMART OS SIMULATOR v2.0        |\n");
    printf("|           Integrated Lab              |\n");
    printf("=========================================\n");
    printf(RESET);
    sleep(1);

    printf("\nLoading");
    for(int i = 0; i < 5; i++) {
        printf("."); fflush(stdout); sleep(1);
    }
    printf("\n");
    sleep(1);
    clear_screen();
}

// Main menu display
void main_menu() {
    printf(CYAN "========== OS SIMULATOR MAIN MENU ==========\n" RESET);
    printf("[1] Individual Module Testing\n");
    printf("[2] Integrated Flow Mode\n");
    printf("[3] Exit\n");
    printf("Select Option: ");
}

int main() {
    char choice;
    char again;

    boot_screen();

    do {
        clear_screen();
        main_menu();
        scanf(" %c", &choice);

        clear_screen();

        switch(choice) {
            case '1':
                printf(GREEN "\n--- Individual Module Testing ---\n" RESET);
                sleep(1);
                individual_module_testing();
                break;

            case '2':
                printf(GREEN "\n--- Integrated Flow Mode ---\n" RESET);
                sleep(1);
                integrated_flow_mode();
                break;

            case '3':
                printf(YELLOW "\nExiting OS Simulator...\n" RESET);
                sleep(1);
                clear_screen();
                exit(0); // Clean exit

            default:
                printf(RED "\nInvalid choice! Please try again.\n" RESET);
                sleep(1);
                continue; // Return to menu prompt
        }

        // Ask user if they want to perform another task
        printf(MAGENTA "\nDo you want to perform another task? (y/n): " RESET);
        scanf(" %c", &again);

    } while(again == 'y' || again == 'Y');

    printf(YELLOW "\nThank you for using OS Simulator! Exiting...\n" RESET);
    sleep(1);
    clear_screen();

    return 0;
}
