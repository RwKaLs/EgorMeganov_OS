#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdbool.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_REPEAT 2

typedef struct input_event input_event;

int main() {
    // input_event structure and file descriptor
    input_event ev;
    int fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
    FILE *file = fopen("ex1.txt", "w");

    // print the available shortcuts
    printf("Available shortcuts:\n"
           "P+E = I passed the Exam!\n"
           "C+A+P = Get some cappuccino!\n"
           "I+U = Innopolis University\n"
           "E+X = <Leave the program>\n");
    fprintf(file, "Available shortcuts:\n"
            "P+E = I passed the Exam!\n"
            "C+A+P = Get some cappuccino!\n"
            "I+U = Innopolis University\n"
            "E+X = <Leave the program>\n");

    // bool variable to detect the X key (for the further exit)
    bool detectX;

    // an infinite loop to read the keyboard events
    while (1) {
        read(fd, &ev, sizeof(input_event));

        // If the event is a key press/repeat/release, print and write it to the file
        if(ev.type == EV_KEY && ev.value == KEY_PRESS) {
            printf("PRESSED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
            fprintf(file, "PRESSED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
        } else if (ev.type == EV_KEY && ev.value == KEY_RELEASE) {
            printf("RELEASED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
            fprintf(file, "RELEASED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
        } else if (ev.type == EV_KEY && ev.value == KEY_REPEAT) {
            printf("REPEATED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
            fprintf(file, "REPEATED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
        }

        // If the E key is pressed, start a loop to detect the X key
        if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_E) {
            while (1) {
                // If the E key is released, break the loop
                if (ev.type == EV_KEY && ev.value == KEY_RELEASE && ev.code == KEY_E) {
                    break;
                }
                read(fd, &ev, sizeof(struct input_event));
                // If the X key is pressed, print "EXIT", set detectX to true, and break the loop
                if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_X) {
                    printf("EXIT\n");
                    fprintf(file, "EXIT\n");
                    detectX = true;
                    break;
                }
            }
            // E+X detected - exit the program
            if (detectX) break;
        }

        // The following blocks of code are similar to the above, but for different shortcuts
        // If the P key is pressed, start a loop to detect the E key
        if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_P) {
            while (1) {
                // If the P key is released, break the loop
                if (ev.type == EV_KEY && ev.value == KEY_RELEASE && ev.code == KEY_P) {
                    break;
                }
                read(fd, &ev, sizeof(struct input_event));
                // If the E key is pressed, print "I passed the Exam!" and break the loop
                if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_E) {
                    printf("I passed the Exam!\n");
                    fprintf(file, "I passed the Exam!\n");
                    break;
                }
            }
        }

        // If the C key is pressed, start a loop to detect the A key
        if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_C) {
            while (1) {
                read(fd, &ev, sizeof(struct input_event));
                // If the A key is pressed, print "A PRESSED" and start a loop to detect the P key
                if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_A) {
                    printf("A PRESSED\n");
                    while (1) {
                        read(fd, &ev, sizeof(struct input_event));
                        // If the P key is pressed, print "Get some cappuccino!" and break the loop
                        if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_P) {
                            read(fd, &ev, sizeof(struct input_event));
                            printf("Get some cappuccino!\n");
                            fprintf(file, "Get some cappuccino!\n");
                            break;
                        }
                        // If the A key is released, break the loop
                        if (ev.type == EV_KEY && ev.value == KEY_RELEASE && ev.code == KEY_A) {
                            break;
                        }
                    }
                }
                // If the C key is released, break the loop
                if (ev.type == EV_KEY && ev.value == KEY_RELEASE && ev.code == KEY_C) {
                    break;
                }
            }
        }

        // If the I key is pressed, start a loop to detect the U key
        if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_I) {
            while (1) {
                // If the I key is released, break the loop
                if (ev.type == EV_KEY && ev.value == KEY_RELEASE && ev.code == KEY_I) {
                    break;
                }
                read(fd, &ev, sizeof(struct input_event));
                // If the U key is pressed, print "Innopolis University" and break the loop
                if (ev.type == EV_KEY && ev.value == KEY_PRESS && ev.code == KEY_U) {
                    printf("Innopolis University\n");
                    fprintf(file, "Innopolis University\n");
                    break;
                }
            }
        }
    }

    // Close the file and the device
    close(fd);
    fclose(file);
    return 0;
}
