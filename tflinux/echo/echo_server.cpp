#include <iostream>
#include "../dns.hpp"
#include "../socket.hpp"
using namespace std;

int main()
{
    SocketAddress clientSocketAddress;

    SocketAddress listenedAddress;
    listenedAddress.setAnyAddress();
    listenedAddress.setProtocolFamily();
    listenedAddress.setPort(4000);

    std::cout << listenedAddress;

    Socket listenedSocket;
    listenedSocket.bindAddress(listenedAddress);
    listenedSocket.setSocketOption();
    listenedSocket.setBaglog();

    int connectedFd = listenedSocket.acceptConnectedDescriptor(clientSocketAddress);



    std::cout << "connected client:\n";
    std::cout << clientSocketAddress;
    char buf[100] = {0};
    int n;
    while ((n = read(connectedFd, buf, 100)) > 0)
    {
        std::cout << buf;
        write(connectedFd, buf, n);
    }

    return 0;
}

