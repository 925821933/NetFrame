#include<iostream>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
#include"../src/http.h"
#include<string.h>
#include<string>


int main(int argc, char const *argv[])
{

  char p[1024];
  memset(p, '\0', 1024);
  htb::HttpRequest::ptr httpRq( new htb::HttpRequest());
  char *req1 ="HEAD /home/huangtianbo/cpp2/cpp/NetFrame/txt.txt HTTP/1.1\r\nHost:www.baidu.com\r\nContent-Length:0\r\nConnection:keep-alive\r\n\r\n";
  int len1 = strlen(req1);
  // for( int i=0; i<strlen(req1) ; i++)
  // {
  //     p[i]=req1[i];
  // }
  // std::string a(p);
  // std::cout<< a<<std::endl;

  // std::cout<<"length:"<<reqlen1<<std::endl;
  char *req2 = "HEAD / HTTP/1.1\r\nHost:www.baidu.com\r\nContent-Length:0\r\nConnection:close\r\n\r\n";
  int len2 = strlen(req2);
  char *req3 = "OPTIONS /home/huangtianbo/cpp2/cpp/NetFrame/text.txt HTTP/1.1\r\nhost:www.baidu.com\r\nContent-Length:0\r\nConnection:keep-alive\r\n\r\n";
  int len3 = strlen(req3);
  httpRq->parse( req3,len3);
  std::string response = httpRq->response();
  std::cout<<response<<std::endl;
}
