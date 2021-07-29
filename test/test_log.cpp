#include<sys/unistd.h>
#include"../src/log.h"
#include<sys/time.h>
#include<pthread.h>
std::string form="[%d] [%r] [%p] [%a] [%l]";
htb::LogFormatter::ptr format( new htb::LogFormatter(form));                                                    //格式
htb::StdAppender::ptr a (new htb::StdAppender());                                                               //输出地点
htb::Logger mylog(htb::INFO, a, format);                                                                        //标准输出

htb::LogMQ::ptr mq( new htb::LogMQ("/home/huangtianbo/cpp2/cpp/NetFrame/test/test_log2.log",5,5000));           //异步输出

htb::FileAppender::ptr b( new htb::FileAppender("/home/huangtianbo/cpp2/cpp/NetFrame/test/test_log.log"));      //同步文件输出
htb::Logger filelog(htb::INFO, b, format);                                                                      //同步文件输出

double t=0;
pthread_mutex_t mutex;
//#define Log(x) mylog.log(__DATE__, __FILE__, std::to_string(__LINE__), x)

void* threadfun( void *arg )
{
    struct timeval begin,end;
    //htb::AsyAppender::ptr c( new htb::AsyAppender(mq)); 
    //htb::Logger Asylog(htb::INFO, c, format);     
    gettimeofday(&begin, NULL);


    htb::AsyLogger myAsylog(form, mq);

    for(int i=0;i<10000;i++)
    {
       myAsylog.log(__DATE__, __FILE__, std::to_string(__LINE__), htb::INFO)<<std::to_string(i)<<"test"<<std::endl;
        //mq->produce("test"+std::to_string(i));
    }
    gettimeofday(&end, NULL);
    int ms1 = (begin.tv_sec*1000 + begin.tv_usec/1000);
    int ms2 = (end.tv_sec*1000 + end.tv_usec/1000);
    double MYt = (ms2-ms1)*1.0/1000;
    std::cout<<"run time is "<<MYt<<"s "<<std::endl;

    pthread_mutex_lock(&mutex);
    t = MYt+t;
    pthread_mutex_unlock(&mutex);
}
int main(int argc, char const *argv[])
{
    
    mylog.log(__DATE__, __FILE__, std::to_string(__LINE__), htb::INFO)<<"this is a std test"<<std::endl;
    filelog.log(__DATE__, __FILE__, std::to_string(__LINE__), htb::INFO)<<"this is a filelog test"<<std::endl;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_t pid1, pid2, pid3;
    pthread_create( &pid1, NULL, threadfun, NULL);
    pthread_create( &pid2, NULL, threadfun, NULL);
    pthread_create( &pid3, NULL, threadfun, NULL);


    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(pid3, NULL);

    std::cout<<(30000*1.0/t)<<std::endl;

}
