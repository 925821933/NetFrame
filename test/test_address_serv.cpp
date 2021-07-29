#include"../src/address.h"
#include<iostream>

int main(int argc, char const *argv[])
{
    htb::IPv4Address::ptr add( new htb::IPv4Address("10.214.199.206", 68888));
    htb::IPv4Socket::ptr listenSock(new htb::IPv4Socket(SOCK_STREAM));
    listenSock->bind( add );
    listenSock->listen();
    std::cout<<"begin to listen!"<<std::endl;
    htb::IPv4Socket::ptr connSock = listenSock->accept();
    std::cout<<"accept connect"<<std::endl;

    char buff[1024];
    memset(buff, '\0', 1024);
    int ret = recv(connSock->getSocket(), buff, 1023, 0);

    std::cout<<"recv:"<<ret<<"bytes    "<<buff<<std::endl;

    char *s = "huangtianbo\r\n";
    std::cout<<strlen(s)<<std::endl;
    ret = send( connSock->getSocket(), s, strlen(s), 0);
    //shutdown( connSock->getSocket(), SHUT_WR);
    std::cout<<"send: "<<ret<<"  bytes"<<std::endl;
    close(connSock->getSocket());
    return 0;
}
