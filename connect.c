/*************************************************************************
	> File Name: connect.c
	> Author: 
	> Mail: 
	> Created Time: 2015年04月15日 星期三 14时13分37秒
 ************************************************************************/


#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFF_SIZE 1024

int main()
{
    int cfd;
    int recbytes;
    int sin_size;
    char buffer[BUFF_SIZE]={0};   
    struct sockaddr_in s_add,c_add;
    unsigned short portnum=3001; 

    printf("Hello,welcome to client !\r\n");

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
            printf("socket fail ! \r\n");
            return -1;

    }
    printf("socket ok !\r\n");

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr= inet_addr("127.0.0.1");
    s_add.sin_port =htons(portnum);
    printf("s_addr = %d ,port : %d\r\n",s_add.sin_addr.s_addr,s_add.sin_port);


    if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
            printf("connect fail !\r\n");
            return -1;

    }
    printf("connect ok !\r\n");

    if(-1 == (recbytes = read(cfd,buffer,1024)))
    {
            printf("read data fail !\r\n");
            return -1;

    }
    printf("read ok\r\nREC:\r\n");

    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);

    printf("send message to ser2net : \n");

    char sline[BUFF_SIZE]={0};
    fgets(sline,BUFF_SIZE,stdin);

    if(send(cfd,sline,strlen(sline),0)<0)
    {
        printf("send msg wrong error: %s(no:%d)\n",strerror(errno),errno);
        exit(0);
    }    

    getchar();
    close(cfd);
    return 0;

}
