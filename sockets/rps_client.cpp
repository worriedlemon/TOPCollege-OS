#include <iostream>
#include <string>

#include "rps_common.h"

/**
 * This program demonstrates sockets by implementing
 * a simple online game Rock-Paper-Scissors between
 * two players.
 * 
 * This is client side.
 */

int main()
{
    Startup();

    // Get server IP
    std::string server_ip;
    std::cout << "Enter server IP (127.0.0.1 for localhost): ";
    std::cin >> server_ip;

    // Create socket
    socket_t client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Connect to server with the specific address family
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    GetServerAddress(server_ip.c_str(), server_addr.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed" << std::endl;
        closesocket(client_socket);
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Receive player ID and initial command
    GameMessage msg;
    int bytes_received = recv(client_socket, BUFFER(msg));
    if (bytes_received <= 0)
    {
        std::cerr << "Failed to receive initial message" << std::endl;
        closesocket(client_socket);
        return 1;
    }

    uint8_t player_id = msg.player_id;
    std::cout << "You are Player " << (int)player_id << std::endl;
    std::cout << "Waiting for other players to connect..." << std::endl;

    // Wait for START command
    bytes_received = recv(client_socket, BUFFER(msg));
    if (bytes_received <= 0 || msg.command != Command::START)
    {
        std::cerr << "Failed to receive start command" << std::endl;
        closesocket(client_socket);
        return 1;
    }

    // Make move
    char choice;

    while (true)
    {
        std::cout << "\nEnter your move (R for Rock, P for Paper, S for Scissors): ";
        std::cin >> choice;

        choice = static_cast<char>(std::tolower(choice));
        if (choice == 'r' || choice == 'p' || choice == 's')
        {
            break;
        }
        std::cout << "Invalid choice! Please try again." << std::endl;
    }

    // Send move to server
    GameMessage move_msg{
        .command = Command::START,
        .player_id = player_id,
        .move = static_cast<Move>(choice),
        .winner_id = 0
    };

    send(client_socket, BUFFER(move_msg));
    std::cout << "Move sent! Waiting for results..." << std::endl;

    // Receive game result
    bytes_received = recv(client_socket, BUFFER(msg));
    if (bytes_received <= 0 || msg.command != Command::GAME_RESULT)
    {
        std::cerr << "Failed to receive game result" << std::endl;
        closesocket(client_socket);
        return 1;
    }

    // Display result
    std::cout << "\n=== GAME RESULT ===" << std::endl;
    if (msg.winner_id == -1)
    {
        std::cout << "It's a TIE!" << std::endl;
    }
    else if (msg.winner_id == player_id)
    {
        std::cout << "YOU WON!" << std::endl;
    }
    else
    {
        std::cout << "You LOST! Player " << (int)msg.winner_id << " won." << std::endl;
    }

    closesocket(client_socket);

    Cleanup();

    return 0;
}
