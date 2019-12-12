#pragma once

#include <iostream>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib");
#define _WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#else if
#include <sys/socket.h> // for socket
#include <netinet/ip.h>
#endif