#include <iostream>
#include <cstring>
#include <string_view>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/**
 * This program is showing the use of anonymous pipe by
 * presenting a very simple shell, which executes commands
 * and prints their output on the screen.
 *
 * Pipes are used to connect parent and child processes in
 * such way (usually) that parent can obtain data from
 * child and somehow process it.
 *
 */

int main()
{
    // Pipes are used to obtain data from other processes
    int pipefd[2];

    std::string cmd, temp;
    while (true)
    {
        std::cout << "Your command:\n> ";

        // Getting command from input
        std::vector<std::string> args;
        std::getline(std::cin, cmd);

        std::istringstream ss(cmd);
        while (std::getline(ss, temp, ' '))
        {
            args.push_back(std::move(temp));
        }

        std::vector<char*> argv;
        argv.reserve(args.size() + 1);
        for (auto& s : args)
        {
            argv.emplace_back(s.data());
        }
        argv.emplace_back(nullptr);

        if (std::cin.eof())
        {
            break;
        }
       
        // Creating pipe
        int rv = pipe(pipefd);
        if (rv == -1)
        {
            std::cerr << "Failed to create pipe (" << std::strerror(errno) << "), terminating..." << std::endl;
        }

        // Duplicating process
        pid_t process = fork();
        if (process == -1)
        {
            std::cerr << "Failed to fork (" << std::strerror(errno) << "), terminating..." << std::endl;
            return 1;
        }

        if (process == 0)
        {
            // We are in the child process, replacing stdout with pipe write end
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);

            // Executing command, the output will be printed out by the parent
            rv = execvp(argv[0], const_cast<char* const*>(&argv[1]));
            if (rv == -1)
            {
                std::cerr << "Could not execute '" << cmd << "'!" << std::endl;
            }
        }
        else
        {
            // We are in the parent process; reading data from pipe read end
            close(pipefd[1]);
            
            // Reading and outputing data from child process
            std::cout << "[" << process << "] Command output:\n";
            char buf[256];
            while (true)
            {
                ssize_t bytes = read(pipefd[0], buf, sizeof(buf));
                if (bytes == 0)
                {
                    break;
                }
                else if (bytes == -1)
                {
                    std::cerr << "Getting output failed: " << std::strerror(errno) << std::endl;
                    break;
                }
                std::cout << std::string_view{ buf, static_cast<size_t>(bytes) };
            }
            std::cout << '\n' << std::endl;
            
            waitpid(process, &rv, 0);
            close(pipefd[0]);

            std::cout << "[" << process << "] exited with code " << WEXITSTATUS(rv) << '\n' << std::endl;
        }
    }

    std::cout << "Exit" << std::endl;

    return 0;
}
