#include"../src/fiberThread.h"
#include"../src/address.h"
#include"../src/epoll.h"
#include<iostream>
htb::FiberPool::ptr pool( new htb::FiberPool(50));
htb::FiberPool::ptr pool2( new htb::FiberPool(50));
void* threadfun(void * arg)
{
    pool->start();
}
void* threadfun2(void * arg)
{
    pool2->start();
}
int main(int argc, char const *argv[])
{
    pthread_t pid, pid2;
    
    pthread_create(&pid, NULL, threadfun,NULL);
    pthread_create(&pid2, NULL, threadfun2,NULL);
    std::cout<<"fiber ready"<<std::endl;

    htb::IPv4Address::ptr myaddress( new htb::IPv4Address("10.214.199.206", 68890));
    htb::IPv4Socket::ptr listenSock( new htb::IPv4Socket(SOCK_STREAM));

    listenSock->bind( myaddress );
    listenSock->listen();


    int epollfd = epoll_create(1);
    htb::EpollFunc::addRecvFdLT(epollfd, listenSock->getSocket());
    epoll_event event[1024];
    while( true )
    {
        int j=0;
        int rt = epoll_wait(epollfd, event, 1024, -1);
        //std::cout<<"epoll nums "<<rt<<std::endl;
        for( int i=0;i<rt;i++)
        {
            if( (event[i].data.fd == listenSock->getSocket()) && (event[i].events&EPOLLIN) )
            {
                int sock = listenSock->accept2();
                //std::cout<<sock<<" got connection"<<std::endl;
                htb::EpollFunc::setNonBlock( sock );
                htb::EpollFunc::addRecvFdET(epollfd,sock);
            }
            else if( event[i].events & EPOLLIN )
            {
                j=(j+1)%2;
                std::cout<<"ring flag: "<<event[i].data.fd<<std::endl;
                htb::Task task(event[i].data.fd, epollfd);
               if( j == 0 ) 
                pool->addTask(task);
                else
                pool2->addTask(task); 
            }
            else if( event[i].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
            {
                htb::EpollFunc::removeFd(epollfd, event[i].data.fd );
            }
        }
    }


    pthread_join(pid, NULL);
    pthread_join(pid, NULL);
    return 0;
}
