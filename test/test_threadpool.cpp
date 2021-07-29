#include<iostream>
#include"../src/address.h"
#include"../src/epoll.h"
#include"../src/fiberThread.h"
#include<string>

int main(int argc, char const *argv[])
{
    if(argc<=2)
    {
        std::cout<<"parameters number should equal two"<<std::endl;
        return 1;
    }
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    
    htb::threadpool::ptr pool( new htb::threadpool(3,50));
    pool->startPool();
    htb::IPv4Address::ptr myaddress( new htb::IPv4Address(ip, port));
    htb::IPv4Socket::ptr listenSock( new htb::IPv4Socket(SOCK_STREAM));

    listenSock->bind( myaddress );
    listenSock->listen();


    int epollfd = epoll_create(1);
    htb::EpollFunc::addRecvFdLT(epollfd, listenSock->getSocket());

    epoll_event event[1024];
    while( true )
    {    
        int rt = epoll_wait( epollfd, event, 1024, -1);
        for( int i=0 ; i<rt ; i++)
        {
            if( (event[i].data.fd == listenSock->getSocket()) && (event[i].events&EPOLLIN) )
            {
                std::cout<<"get connection"<<std::endl;
                int sock = listenSock->accept2();
                htb::EpollFunc::setNonBlock( sock );
                htb::EpollFunc::addRecvFdET(epollfd,sock);
            }
            else if(event[i].events&EPOLLIN)
            {
               htb::Task task(event[i].data.fd, epollfd);
               pool->addTask( task );
            }
            else if( event[i].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
            {
                htb::EpollFunc::removeFd(epollfd, event[i].data.fd );
            }
        }
        
    }

    return 0;
}
