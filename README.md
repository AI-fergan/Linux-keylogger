# Linux-keylogger

# Set-up
*Compilation*
- Server:
`g++ -o server src/main.cpp keyboard-driver/keyboard.c -pthread`
- Client:
`gcc -o client src/main.c sock/socket.c`