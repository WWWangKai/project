/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    struct sockaddr_in ad; //声明一个ipv4地址类型的结构体
    struct hostent *hp;
    
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;  //设置为流式 这是Tcp协议的传输方式

	
	/*若字符串有效，则将字符串转换为32位二进制。网络字节序的IPV4地址，否则为INADDR_NONe*/
    inaddr = inet_addr(host); //将点分十进制的的ip转化为无符号的长整形
    if (inaddr != INADDR_NONE)
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));  //转换后的ip设置进ad的这个结构体
	
    /*如果host是域名的话就执行下面的操作*/
	else
    {
        hp = gethostbyname(host);//用域名来获取ip地址
		
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);  //设置它的端口号
    
    sock = socket(AF_INET, SOCK_STREAM, 0);  //申请一个套接字
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)  //建立连接
        return -1;
    return sock;
}

