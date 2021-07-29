#include"../src/address.h"
#include<iostream>
#include<pthread.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
class IPandPort
{
    public:
        std::string ip;
        int port;

};
void *threadfunc( void* arg )
{
    IPandPort *thisptr = (IPandPort*)arg;
    int port = thisptr->port;
    std::string ip = thisptr->ip;
    
    htb::IPv4Address::ptr connAddr( new htb::IPv4Address(ip, port));
    for( int i=0;i<10000;i++)
    {
    htb::IPv4Socket::ptr conSock( new htb::IPv4Socket(SOCK_STREAM));
    bool ret = conSock->connect( connAddr );

    char *buf = "OPTIONS /home/huangtianbo/cpp2/cpp/NetFrame/text.txt HTTP/1.1\r\nhost:www.baidu.com\r\nContent-Length:0\r\nConnection:close\r\n\r\n";
    int a = send( conSock->getSocket(), buf, strlen(buf), 0);
    char buff[1024];
    memset(buff,'\0', 1024);
    int rt = recv(conSock->getSocket(), buff, 1023, 0);
    //std::cout<<"recv:"<<rt<<"  bytes "<<buff<<std::endl;
    conSock->close();
    }
}
int main(int argc, char const *argv[])
{
    if(argc<=2)
    {
        std::cout<<"parameters number should equal two"<<std::endl;
        return 1;
    }

    IPandPort myp;
    myp.ip = argv[1];
    myp.port = atoi(argv[2]);

    IPandPort *ptr = &myp;
    
    pthread_t pid1,pid2,pid3;
    
    int port = atoi(argv[1]);
    
        struct timeval begin,end;
 
    /* 获取时间，理论到us */
    gettimeofday(&begin, NULL);
    printf("s: %ld, ms: %ld\n", begin.tv_sec, (begin.tv_sec*1000 + begin.tv_usec/1000));
 
    /* 重新获取 */
    pthread_create(&pid1, NULL,threadfunc, ptr);
    pthread_create(&pid2, NULL,threadfunc, ptr);
    pthread_create(&pid3, NULL,threadfunc, ptr);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(pid3, NULL);
    gettimeofday(&end, NULL);
    printf("s: %ld, ms: %ld\n", end.tv_sec, (end.tv_sec*1000 + end.tv_usec/1000));

    int ms1 = (begin.tv_sec*1000 + begin.tv_usec/1000);
    int ms2 = (end.tv_sec*1000 + end.tv_usec/1000);
    double t = (ms2-ms1)*1.0/1000;
    std::cout<<"run time is "<<t<<"s "<<std::endl;
    std::cout<<"tps is "<<30000/t<<std::endl;
    return 0;
}
