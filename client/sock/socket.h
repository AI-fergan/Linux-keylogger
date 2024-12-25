#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/input.h>

#define PORT 1393

void send_key(struct input_event ev);
