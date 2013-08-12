
#include "../socket.hpp"
#include <stdio.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

int main()
{
    Socket socket;
    SocketAddress serverAddress(ip_t("127.0.0.1"), 4000);
    socket.connectToServer(serverAddress);

    std::string str;
    std::cin >> str;
    std::cout << "send string: " << str << std::endl;
    std::cout << write(socket.descriptor, str.c_str(), str.size()) << " bytes sent!" << std::endl;

    char buf[100] = {0};
    read(socket.descriptor, buf, 100);
    std::cout << buf << std::endl;


//    std::string str;
//    while(std::cin >> str)
//    {
//        if (write(socket.descriptor, str.c_str(), str.size()) != str.size())
//            unix_error("write error");
//        std::cout << "write string!" << std::endl;

//    }

    close(socket.descriptor);
}



