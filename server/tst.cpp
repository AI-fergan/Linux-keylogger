#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define EVENT_FILE "/dev/input/event3"

int main() {
    int fd;
    struct input_event ev;
    char code;

    keyboard_init();

    // Open the event file for reading
    fd = open(EVENT_FILE, O_RDONLY);
    if (fd < 0) {
        perror("Error opening event file");
        return EXIT_FAILURE;
    }

    printf("Monitoring key events from: %s\n", EVENT_FILE);
    printf("Press Ctrl+C to stop.\n\n");

    // Loop to read and print key events
    while (1) {
        ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
        if (bytes < (ssize_t)sizeof(struct input_event)) {
            perror("Error reading event");
            close(fd);
            return EXIT_FAILURE;
        }

        code = 0;
        if (ev.type == EV_KEY) {
            if (ev.value == 1)
                code = ev.code;
            else if (ev.value == 0)
                code = ev.code | RELEASE_CODE;
        }

        key_event event = keyboard_handler(code);
        if (ev.value == 1)
            printf("key %c pressed\n", key_to_ascii(event));
    }

    close(fd);
    return EXIT_SUCCESS;
}
