# Linux Keylogger

A lightweight keylogger with a server-client architecture. The client captures keystrokes using Linux input events and sends them to the server, which logs the data into organized files.

## Set-up

### Compilation
- **Server**:  
  `g++ -o server src/main.cpp keyboard-driver/keyboard.c -pthread`
- **Client**:  
  `gcc -o client src/main.c sock/socket.c`

### Usage
1. Start the server:  
   `./server`
2. Run the client (with necessary permissions):  
   `sudo ./client`

### Logs
- Logs are saved in `logs/` directory with filenames based on the client IP and file index, e.g., `logs/127.0.0.1-log00.txt`.
