#include"../src/fiberThread.h"
#include<iostream>

void fiberTest( int fd)
{
    fd++;
    int flag= false;
    std::cout<<fd<<"fiber1 flase switch to main"<<std::endl;
    flag =true;
    fd++;
    htb::fiber::swapToMain();
    std::cout<<fd<<"fiber1 true switch to main"<<std::endl;
    htb::fiber::swapToMain();
}

void fiberTest2(int fd)
{
    char a[]="nihaoa ";
    int flag= false;
    std::cout<<"fiber2 false switch to main"<<std::endl;
    flag =true;
    htb::fiber::swapToMain();
    a[4]='q';
    std::cout<<a<<"fiber2 true switch to main"<<std::endl;
    htb::fiber::swapToMain();
}

void* threadFunc( void* arg )
{
    htb::Task task(0,0);
    htb::fiber::ptr fiber1( new htb::fiber(nullptr, task) );
    fiber1->setFiberCb(fiberTest);
    std::cout<<"fiber number:"<<htb::fiber::getTotalFiber()<<std::endl;

    std::cout<<"thread get in fiber1"<<std::endl;
    //开始执行fiber
    htb::fiber::swapToBranch( fiber1 );
    std::cout<<"thread out fiber1"<<std::endl;

    // std::cout<<"thread get in fiber2"<<std::endl;
    // htb::fiber::swapToBranch( fiber2 );
    // std::cout<<"thread out fiber2"<<std::endl;

    std::cout<<"thread get in fiber1"<<std::endl;
    htb::fiber::swapToBranch( fiber1 );
    std::cout<<"thread out fiber1"<<std::endl;

    // std::cout<<"thread get in fiber2"<<std::endl;
    // htb::fiber::swapToBranch( fiber2 );
    // std::cout<<"thread out fiber2"<<std::endl;

    fiber1->reset(task);
    std::cout<<"thread get in fiber1"<<std::endl;
    htb::fiber::swapToBranch( fiber1 );
    std::cout<<"thread out fiber1"<<std::endl;

    std::cout<<"thread get in fiber1"<<std::endl;
    htb::fiber::swapToBranch( fiber1 );
    std::cout<<"thread out fiber1"<<std::endl;

}

int main(int argc, char const *argv[])
{
    pthread_t pid;
    pthread_create(&pid, nullptr, threadFunc, nullptr);
    pthread_join( pid, nullptr);
    std::cout<<"main func"<<std::endl;

}
