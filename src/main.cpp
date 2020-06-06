#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main()
{
    // Create a socket

    int listening = socket(AF_INET, SOCK_STREAM, 0); // arguments: address family(IPv4), type- socket stream, protocol 0 and will be determined by networking stack
    if (listening == -1)
    {
        std::cerr << "Can't create a socket!";
        return -1;
    }

    // Bind the socket to the IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);                  // htons: host to network short - flips bits depending if little endian or big endian (how sysytem stores number bigger than 255)
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // internet command, inverts number to array of integers 127.0.0.1 converts to array

    if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) // bind hint to the port
    {
        std::cerr << "Can't bind to IP/port" << std::endl;
        return -2;
    }
    // Mark the socket for listening in
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen!" << std::endl;
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening,
                              (sockaddr *)&client,
                              &clientSize); // acccept an incoming connection on listening socket
    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting!" << std::endl;
        return -4;
    }

    // Close the listening socket
    close(listening);

    memset(host, 0, NI_MAXHOST); //clean things up
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client,
                             sizeof(client),
                             host,
                             NI_MAXHOST,
                             svc,
                             NI_MAXSERV,
                             0); // try to get client computer name, if fails, returns 0

    if (result)
    {
        std::cout << host << " connected on " << svc << std::endl;
    }
    else // if we don't get name of the host, we do things manualy
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // oposite of inet_pton
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    // While receiving display message, echo message
    char buf[4096];
    while (true)
    {
        // Clear the buffer
        memset(buf, 0, 4096);

        // Wait for a message
        int bytesRecv = recv(clientSocket, buf, 4096, 0); // the socket is no longer the listening socket, it's a client socket
        if (bytesRecv == -1)
        {
            std::cerr << "There was a connection issue!" << std::endl;
            break;
        }
        if (bytesRecv == 0)
        {
            std::cout << "The client disconnected" << std::endl;
            break;
        }
        // Display message
        std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
        // Resend message
        send(clientSocket, buf, bytesRecv + 1, 0); // +1 because we have to have 0 in the end
    }

    //Close the socket
    close(clientSocket);

    return 0;
}
