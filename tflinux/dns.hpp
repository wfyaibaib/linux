#ifndef DNS_H
#define DNS_H
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>


struct ip_t : public in_addr
{
    /*
     typedef uint32_t in_addr_t;
     struct in_addr
     {
           in_addr_t s_addr;
     }
  */

    ip_t(){}
    ip_t(in_addr_t uint32num)// from ip address
    {
        s_addr = uint32num;
    }
    ip_t(in_addr addr)// from base struct
    {
        s_addr = addr.s_addr;
    }

    ip_t(const std::string& str)
    {
        inet_aton(str.c_str(), this);
    }
    std::string toString() const
    {
        return inet_ntoa(*this);
    }
    friend std::ostream& operator<<(std::ostream& os, const ip_t& ip)
    {
        os << ip.toString();
        return os;
    }
};

struct dns
{
    typedef hostent hostentry_t;
    hostentry_t entry;

    dns(const char* hostname)
    {
        hostent* p = gethostbyname(hostname);//localhost www.baidu.com
        if (p == NULL)
        {
            std::cerr << "gethostbyname error!";
            exit(-1);
        }
        entry = *p;
    }
    dns(ip_t ip)
    {//
        hostent* p = gethostbyaddr((const char*)(&ip), sizeof(ip),
                               AF_INET);
        if (p == NULL)
        {
            std::cerr << "gethostbyname error!";
            exit(-1);
        }
        entry = *p;
    }

    std::string getOfficialDomainName() const
    {
        return entry.h_name;
    }
    std::vector<std::string> getAliasesDomainName() const
    {
        std::vector<std::string> vec;
        for (char ** alias = entry.h_aliases; *alias != NULL; ++alias)
        {
            vec.push_back(std::string(*alias));
        }
        return vec;
    }
    int getAddressType() const
    {
        return entry.h_addrtype;
    }
    int getLengthOfAddress() const
    {
        return entry.h_length;
    }
    std::vector<ip_t> getAddressList() const
    {
        std::vector<ip_t> ipVec;
        for (char ** p= entry.h_addr_list; *p != NULL; ++p)
        {
            in_addr_t num = ((struct in_addr*)*p)->s_addr;
            ip_t ip;
            ip.s_addr = num;
            ipVec.push_back(ip);
        }
        return ipVec;
    }
    ip_t getOneIp()
    {
        return getAddressList().at(0);
    }

    friend std::ostream& operator<<(std::ostream& os, const dns& entry)
    {
        os << "Official domain name:\n";
        os << entry.getOfficialDomainName() << std::endl;
        os << "Domain name aliases:\n";
        std::vector<std::string> aliases = entry.getAliasesDomainName();
        std::copy(aliases.begin(), aliases.end(), std::ostream_iterator<std::string>(os, "\n"));
        os << "Host address type:\n";
        os << entry.getAddressType() << std::endl;
        os << "Length of an address:\n";
        os << entry.getLengthOfAddress() << std::endl;
        os << "address:\n";
        std::vector<ip_t> ipVec = entry.getAddressList();
        std::copy(ipVec.begin(), ipVec.end(), std::ostream_iterator<ip_t>(os, "\n"));
    }

    // test
    static void test()
    {
       // std::cout << ip_t("61.135.169.125").toString() << std::endl;
        std::cout << dns("www.baidu.com").getOneIp() << std::endl;
    }
};

#endif // DNS_H
