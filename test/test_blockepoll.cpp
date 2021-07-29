#include"../src/blockIOThread.h"
#include<iostream>
int main(int argc, char const *argv[])
{
    htb::BlocKIOThreadPool::ptr pool( new htb::BlocKIOThreadPool(3));
    pool->start();
    htb::IPv4Address::ptr myaddress( new htb::IPv4Address("10.214.199.206", 68890));
    htb::IPv4Socket::ptr listenSock( new htb::IPv4Socket(SOCK_STREAM));

    listenSock->bind( myaddress );
    listenSock->listen();


    int epollfd = epoll_create(1);
    htb::EpollFunc::addRecvFdLT(epollfd, listenSock->getSocket());
    epoll_event event[1024];

    while( true )
    {
        int rt = epoll_wait(epollfd, event, 1024, -1);
        std::cout<<rt<<std::endl;
        for( int i=0 ;i<rt; i++)
        {
            //std::cout<<"begin"<<std::endl;
            if( (event[i].data.fd == listenSock->getSocket()) && (event[i].events&EPOLLIN) )
            {
                int sock = listenSock->accept2();
                 //std::cout<<"got connection: "<<sock<<std::endl;
                htb::EpollFunc::addRecvFdLT(epollfd, sock);
            }
            else if( event[i].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
            {
                 std::cout<<"ring close: "<<event[i].data.fd<<std::endl;
                htb::EpollFunc::removeFd(epollfd, event[i].data.fd );
                //close(event[i].data.fd);
            }
            else if( event[i].events & EPOLLIN )
            {
                std::cout<<"ring flag: "<<event[i].data.fd<<std::endl;
                htb::BlockIOTask task(event[i].data.fd, epollfd);
                pool->addTask( task );
            }
            std::cout<<"end"<<std::endl;
        }
    }
    return 0;
}
