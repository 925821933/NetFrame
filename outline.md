# 轻量级的c++并发http服务器框架
## 1.简介
本项目为C++编写的轻量级的web并发服务器框架，能够简单解析http协议中的**head、get、options**方法。该服务器使用**reactor模式+非阻塞IO+以协程为基础的线程池**的框架，实现了处理并发请求的功能，处理http短链接的QPS约为51000。同时为了支持并发数据的记录，实现了简单的**异步日志系统**来记录服务器的状态，日志并发量能够达到110000/s。
## 2.目录结构及环境
1.环境
+ ubuntu16.04  
+ g++5.5.0  

2.目录结构
+ build     -------Build结果
+ src       -------源代码
+ lib       -------库目录
+ test      -------测试代码目录
+ CMakeLists.txt
+ outline.md
## 3.主要技术点
+ 使用**缓存技术**实现了简单的**异步日志系统**，日志系统是**C++的流式风格**，能够记录日志输出的时间、文件、行号、内容等；
+ 总体是基于**Reactor模式+非阻塞IO+以协程为基础的线程池**框架；
+ Reactor模式的架构基于**epoll多路IO复用技术**，主线程只负责连接客户端，线程池负责处理接收、解写、发送协议。具体架构见 **图Reactor**；
+ 线程池中**提前创建多个线程**，避免频繁创建销毁的开销。为了充分调动各个线程工作，使用**Round-Robin算法**为各个线程分配任务；
+ Epoll注册事件的模式为 **ET|EPOLLONESHOT + 非阻塞IO**；
+ 线程池中的每个线程包含了一个**协程池**，协程池的工作模式：**主协程（负责协程切换）+多个分协程（负责执行任务）**
+ 协程功能的实现基于**ucontext函数族**，协程的状态有**INIT、EXEC、HOLD、TERM**, 主要的功能为**YieldtoBranch、YieldtoHold、YieldtoTerm**
+ 使用状态机的方式，解析了Http协议的请求
+ 大量使用了智能指针RAII技术，减少内存泄露的风险。
## 4.协程池工作示意图
```
             branch1              branch2
           ------------         -----------
   main    ⬆          ⬇   main  ⬆         ⬇   main
------------          -----------         -----------
                              
```
## 5.Reactor

## 线程封装
1.线程 2.互斥锁 3.条件变量
## 日志系统
1. 功能:   
+ c++流式风格
+ 用双缓存队列实现简单的异步功能

+ coredump 信号中断 
2. 类:
## 网络模块

+ 地址类address-----（ipv4 ,ipv6, unix)
+ 句柄类socket -----（create bind listen accept listen  close    tcp recv send  udp recvfrom sendto)
+ tcp/udp 接收发

## http
+ 简单实现 http/1.1 get head optionst方法
+ HttpRequest
+ HttpResponse
```
GET / HTTP/1.1\r\n
Host:www.baidu.com\r\n
Content-Length:0\r\n
Connection:keep-alive\r\n
\r\n

HEAD / HTTP/1.1\r\n
Host:www.baidu.com\r\n
Content-Length:0\r\n
Connection:keep-alive/close\r\n
\r\n

OPTIONS / HTTP/1.1\r\n
host:www.baidu.com\r\n
Content-Length:0\r\n
Connection:keep-alive/close\r\n
\r\n

HTTP1.1 204 No Content\r\n
Allow:GET,HEAD,OPTIONS\r\n
Content-Length:0\r\n
Connection:keep-alive/close\r\n
\r\n

HTTP1.1 200 OK\r\n
Content-type:text\r\n
Content-Length:5\r\n
\r\n
hello

HTTP1.1 404 Not Found\r\n
Content-Length:0\r\n
\r\n

HTTP1.1 400 Bad Request\r\n
Content-Length:0\r\n
\r\n

get head options
Content-Length:


\r\n 回车换行来表示一行的结束
uri:http://www.baidu.com/path?id=#frag
```


待解决：
定时器
优雅关闭



问题1 event[i].events 错了

多线程 fd分配关闭要在一个里面   出现了 bad file discriber  ：添加是线程安全的  如果一个线程中由select管理的socket被另外一个线程close掉，将会发生什么只有天晓得。在一些UNIX系统中，select会结束阻塞态并返回，它会标识这个socket处于ready状态(后面对这个socket的操作会失败，os也会给出错误提示， 在其他线程中删除 感兴趣的fd，会出现多个连接用一个socket的问题， close以后出现 bad file discriber

不关的话大概400多个fd 就出现tempro

epollonshotET+协程
epollonshotLT+线程池


流写入很久
