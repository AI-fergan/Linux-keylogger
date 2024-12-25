#include "socket.h"

/*
This function sends the input_event to the server.
input:
ev - the input_event to send to the server.
output: None.
*/
void send_key(struct input_event ev) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    //create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //convert IPv4 address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sock);
        return;
    }

    //connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return;
    }

    //send the input_event structure directly
    if (send(sock, &ev, sizeof(ev), 0) < 0) {
        perror("Send failed");
    }

#ifdef DBG
    printf("Sent input_event: type=%d, code=%d, value=%d\n", ev.type, ev.code, ev.value);
#endif

    close(sock);
}
