#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "rps_common.h"

/**
 * This program demonstrates sockets by implementing
 * a simple online game Rock-Paper-Scissors between
 * two players.
 *
 * This is server side.
 */

static int8_t determineWinner(Move p1, Move p2)
{
    if (p1 == p2)
    {
        return -1;
    }

    return ((p1 == Move::ROCK && p2 == Move::SCISSORS) ||
           (p1 == Move::PAPER && p2 == Move::ROCK) ||
           (p1 == Move::SCISSORS && p2 == Move::PAPER))
           ? 0 : 1;
}


int main()
{
    Startup();

    // Create server socket, we well use TCP transport protocol (type SOCK_STREAM)
    socket_t server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // We need to bind socket to a specific address family
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_socket);
        return 1;
    }

    // Listen
    if (listen(server_socket, 2) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_socket);
        return 1;
    }

    std::cout << "Server started on port " << PORT << std::endl;
    std::cout << "Waiting for 2 players to connect..." << std::endl;

    // Accept connections
    std::vector<socket_t> clients;
    for (int i = 0; i < 2; ++i)
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        socket_t client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        clients.push_back(client_socket);
        std::cout << "Player " << i << " connected from " << inet_ntoa(client_addr.sin_addr) << std::endl;

        // Send player ID and WAIT command
        GameMessage msg{
            .command = Command::WAIT,
            .player_id = static_cast<uint8_t>(i),
            .move = Move::NONE,
            .winner_id = 0
        };
        send(client_socket, BUFFER(msg));
    }

    std::cout << "All players connected! Starting game..." << std::endl;

    // Send START command to all players
    GameMessage start_msg{
        .command = Command::START,
        .player_id = 0,
        .move = Move::NONE,
        .winner_id = 0,
    };

    for (socket_t client : clients)
    {
        send(client, BUFFER(start_msg));
    }

    // Receive moves from players
    std::vector<Move> moves(2);
    std::cout << "Waiting for players' moves..." << std::endl;

    for (int i = 0; i < 2; ++i)
    {
        GameMessage msg;
        int bytes_received = recv(clients[i], BUFFER(msg));
        if (bytes_received > 0)
        {
            moves[i] = msg.move;
            std::cout << "Player " << (int)i << " made their move" << std::endl;
        }
    }

    int8_t winner = determineWinner(moves[0], moves[1]);
    std::cout << "Winner is: " << (winner >= 0 ? std::to_string(winner) : "none") << std::endl;

    // Send result to all players
    GameMessage result_msg{
        .command = Command::GAME_RESULT,
        .player_id = 0,
        .move = Move::NONE,
        .winner_id = winner
    };

    for (socket_t client : clients)
    {
        send(client, BUFFER(result_msg));
        closesocket(client);
    }
    closesocket(server_socket);

    Cleanup();

    return 0;
}
