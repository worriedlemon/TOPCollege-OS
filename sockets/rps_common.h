#pragma once

#ifndef RPS_COMMON_H
#define RPS_COMMON_H

#define BUFFER(x) (char*)&x, sizeof(x), 0

#if defined(_WIN32) || defined(WIN32)

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
typedef SOCKET socket_t;

#define Startup() \
WSADATA wsaData; \
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) \
{ \
    std::cerr << "WSAStartup failed" << std::endl; \
    return 1; \
}

#define Cleanup() WSACleanup()

#define GetServerAddress(str, sin_addr) inet_pton(AF_INET, str, &sin_addr);

#else // UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
typedef int socket_t;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close

#define Startup()

#define Cleanup()

#define GetServerAddress(str, sin_addr) sin_addr.s_addr = inet_addr(str);

#endif // _WIN32

#include <cstdint>

constexpr int PORT = 8888;

enum class Move : uint8_t
{
    NONE = '\0',
    ROCK = 'r',
    PAPER = 'p',
    SCISSORS = 's'
};

enum class Command : uint8_t
{
    WAIT = 0,
    START = 1,
    GAME_RESULT = 2
};

#pragma pack(push, 1)
struct GameMessage
{
    Command command;
    uint8_t player_id;
    Move move;
    int8_t winner_id;
};
#pragma pack(pop)

#endif // RPS_COMMON_H
