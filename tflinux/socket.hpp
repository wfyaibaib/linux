#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include "unix_error.hpp"
#include "dns.hpp"

struct SocketAddress : public sockaddr_in
{
    unsigned short getProtocolFamily() const
    {
        return sin_family;
    }
    void setProtocolFamily()
    {
        this->sin_family = AF_INET;
    }

    ip_t getIp() const
    {
        return sin_addr;// len:14
    }

    SocketAddress(){}
    SocketAddress(ip_t ip, unsigned short port)
    {
        this->sin_family = AF_INET;
        this->sin_port = htons(port);
        this->sin_addr = ip;// up cast
    }
    void setAnyAddress()
    {
        this->sin_addr = ip_t(htonl(INADDR_ANY));
    }
    void setAddress(ip_t ip)
    {
        this->sin_addr = ip;
    }
    void setPort(unsigned short port)
    {
        this->sin_port = htons(port);
    }
    unsigned short getPort() const
    {
        return ntohs(this->sin_port);
    }

    friend std::ostream& operator<<(std::ostream& os, const SocketAddress& addr)
    {
        os << "SocketAddress:\n";
        os << addr.getIp() << std::endl;
        os << addr.getPort() << std::endl;
    }
};

struct Socket
{
    int descriptor;

    Socket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0)
    {
        descriptor = socket(domain, type, protocol);

        if (descriptor == -1)
            unix_error("socket error");
    }
    void connectToServer(SocketAddress serverAddress)// client socket
    {
        if (
                connect(descriptor,
                       reinterpret_cast<sockaddr*>(&serverAddress),
                       sizeof(serverAddress)) < 0
           )
            unix_error("connect error");
    }
    void bindAddress(SocketAddress addr)// to be a listening socket
    {
        if (bind(descriptor,
                 reinterpret_cast<sockaddr*>(&addr),
                sizeof(addr)) < 0)
            unix_error("bind error");
    }
    void setBaglog(int backlog = 1024)
    {
        if (listen(descriptor, backlog) < 0)
            unix_error("listen error");
    }
    void setSocketOption()
    {
        int opt = 1;
        if (setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&opt, sizeof(int)) < 0)
            unix_error("setsockopt error");
    }
    int acceptConnectedDescriptor(SocketAddress& peerSocketAddress)
    {
        int connectedDescriptor;
        unsigned socketLen = sizeof(SocketAddress);
        connectedDescriptor =  accept(descriptor,
                   reinterpret_cast<sockaddr*>(&peerSocketAddress),
                    &socketLen);
        if (connectedDescriptor < 0)
            unix_error("accept error!");
        return connectedDescriptor;
    }


};

#endif // SOCKET_HPP
