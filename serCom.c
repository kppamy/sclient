/*************************************************************************
> File Name: setSpeed.c
> Author: 
> Mail: 
> Created Time: 2015年04月15日 星期三 11时39分26秒
************************************************************************/

#include<stdio.h>

#include <sys/types.h>  

#include <sys/stat.h>  
#include <fcntl.h>  
#include <termios.h>  
#include <stdio.h>  
#define BAUDRATE        B115200  
#define UART_DEVICE     "/dev/ttyS0"  
#define BUFF_SIZE 1024
#define FALSE  -1  
#define TRUE   0  
////////////////////////////////////////////////////////////////////////////////  
///** 
//*@brief  设置串口通信速率 
//*@param  fd     类型 int  打开串口的文件句柄 
//*@param  speed  类型 int  串口速度 
//*@return  void 
//*/  
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,  
               B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };  
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300,   
              115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, };  
void set_speed(int fd, int speed){  
int   i;   
int   status;   
struct termios   Opt;  
tcgetattr(fd, &Opt);   
for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++ ) {   
    if  (speed == name_arr[i]) {       
        tcflush(fd, TCIOFLUSH);       
        cfsetispeed(&Opt, speed_arr[i]);    
        cfsetospeed(&Opt, speed_arr[i]);     
        status = tcsetattr(fd, TCSANOW, &Opt);    
        if  (status != 0) {          
            perror("tcsetattr fd1");    
            return;       
        }      
        tcflush(fd,TCIOFLUSH);     
    }    
}  
}  

////////////////////////////////////////////////////////////////////////////////  
///** 
//*@brief   设置串口数据位，停止位和效验位 
//*@param  fd     类型  int  打开的串口文件句柄 
//*@param  databits 类型  int 数据位   取值 为 7 或者8 
//*@param  stopbits 类型  int 停止位   取值为 1 或者2 
//*@param  parity  类型  int  效验类型 取值为N,E,O,,S 
//*/  
int set_Parity(int fd,int databits,int stopbits,int parity)  
{   
    struct termios options;   
    if  ( tcgetattr( fd,&options )  !=  0 ) {   
        perror("SetupSerial 1");       
        return(FALSE);    
    }  
    options.c_cflag &= ~CSIZE;   
    switch (databits) /*设置数据位数*/  
    {     
        case 7:       
        options.c_cflag |= CS7;   
        break;  
        case 8:       
        options.c_cflag |= CS8;  
        break;     
        default:      
        fprintf(stderr,"Unsupported data size\n"); return (FALSE);    
    }  
    switch (parity)   
    {     
        case 'n':  
        case 'N':      
        options.c_cflag &= ~PARENB;   /* Clear parity enable */  
        options.c_iflag &= ~INPCK;     /* Enable parity checking */   
        break;    
        case 'o':     
        case 'O':       
        options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/    
        options.c_iflag |= INPCK;             /* Disnable parity checking */   
        break;    
        case 'e':    
        case 'E':     
        options.c_cflag |= PARENB;     /* Enable parity */      
        options.c_cflag &= ~PARODD;   /* 转换为偶效验*/       
        options.c_iflag |= INPCK;       /* Disnable parity checking */ 
        break;  
        case 'S':   
        case 's':  /*as no parity*/     
        options.c_cflag &= ~PARENB;  
        options.c_cflag &= ~CSTOPB;break;    
        default:     
        fprintf(stderr,"Unsupported parity\n");      
        return (FALSE);    
    }    

    /* 设置停止位*/    
    switch (stopbits)  
    {     
        case 1:      
        options.c_cflag &= ~CSTOPB;    
        break;    
        case 2:      
        options.c_cflag |= CSTOPB;    
        break;  
        default:      
        fprintf(stderr,"Unsupported stop bits\n");    
        return (FALSE);   
    } 

   /* Set input parity option */   
    if (parity != 'n')     
    options.c_iflag |= INPCK;   
    tcflush(fd,TCIFLUSH);  
    options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/     
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */  

    if (tcsetattr(fd,TCSANOW,&options) != 0)     
    {   
        perror("SetupSerial 3");     
        return (FALSE);    
    }   

    options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/  
    options.c_oflag  &= ~OPOST;   /*Output*/  
    return (TRUE);    
}  



void
read_nonblock(int fd)
{
    fd_set      tmp_read_set;
   // fd_set      tmp_write_set;
   // fd_set      tmp_except_set;
    int         i;
    int         err;
   // sel_timer_t *timer;
    struct timeval timeout, *to_time;
    char* rbuf[BUFF_SIZE]={0};

   // if (sel->timer_top) 
   // {
   //     struct timeval now;

   //     /* Check for timers to time out. */
   //     gettimeofday(&now, NULL);
   //     timer = sel->timer_top;
   //     while (cmp_timeval(&now, &timer->timeout) >= 0) 
   //     {
   //         remove_from_heap(&(sel->timer_top),
   //                          &(sel->timer_last),
   //                          timer);

   //         timer->in_heap = 0;
   //         timer->handler(sel, timer, timer->user_data);

   //         timer = sel->timer_top;
   //         gettimeofday(&now, NULL);
   //         if (!timer)
   //         goto no_timers;
   //     }

   //     /* Calculate how long to wait now. */
   //     diff_timeval(&timeout, &sel->timer_top->timeout, &now);
   //     to_time = &timeout;
   // } else 
   // {
   //     no_timers:
   //     to_time = NULL;
   // }
   // memcpy(&tmp_read_set, &sel->read_set, sizeof(tmp_read_set));
   // memcpy(&tmp_write_set, &sel->write_set, sizeof(tmp_write_set));
   // memcpy(&tmp_except_set, &sel->except_set, sizeof(tmp_except_set));
    //err = select(sel->maxfd+1,&tmp_read_set,&tmp_write_set,&tmp_except_set,to_time);
    FD_ZERO(&tmp_read_set);
    FD_SET(fd,&tmp_read_set);
    while(FD_ISSET(fd,&tmp_read_set))
    {
        FD_ZERO(&tmp_read_set);
        FD_SET(fd,&tmp_read_set);
        err = select(fd+1,&tmp_read_set,NULL,NULL,0);
        if (err == 0) 
        {
            /* A timeout occurred. */
        } else if (err < 0) 
        {
            /* An error occurred. */
            printf("select error:\n");
           // if (errno == EINTR) 
           // {
           //     /* EINTR is ok, just restart the operation. */
           //     timeout.tv_sec = 1;
           //     timeout.tv_usec = 0;
           // } else 
           // {
           //     /* An error is bad, we need to abort. */
           //    // syslog(LOG_ERR, "select_loop() - select: %m");
           //     printf("select error:\n");
           //     exit(1);
           // }
        } else 
        {
            /* We got some I/O. */
            int ret=read(fd,rbuf,BUFF_SIZE);
            if(ret>0)
            {
                printf("receiving data from serial:\n  %s\n",rbuf);
            }
           // for (i=0; i<=sel->maxfd; i++) 
           // {
           //     if (FD_ISSET(i, &tmp_read_set)) 
           //     {
           //         if (sel->fds[i].handle_read == NULL) 
           //         {
           //             /* Somehow we don't have a handler for this.
           //             Just shut it down. */
           //             sel_set_fd_read_handler(sel, i, SEL_FD_HANDLER_DISABLED);
           //         } else 
           //         {
           //             sel->fds[i].handle_read(i, sel->fds[i].data);
           //         }
           //     }
           //   if (FD_ISSET(i, &tmp_write_set)) 
           //   {
           //       if (sel->fds[i].handle_write == NULL) 
           //       {
           //           /* Somehow we don't have a handler for this.
           //           Just shut it down. */
           //           sel_set_fd_write_handler(sel, i, SEL_FD_HANDLER_DISABLED);
           //       } else 
           //       {
           //           sel->fds[i].handle_write(i, sel->fds[i].data);
           //       }
           //   }
           //   if (FD_ISSET(i, &tmp_except_set)) 
           //   {
           //       if (sel->fds[i].handle_except == NULL) 
           //       {
           //           /* Somehow we don't have a handler for this.
           //           Just shut it down. */
           //           sel_set_fd_except_handler(sel, i, SEL_FD_HANDLER_DISABLED);
           //       } else 
           //       {
           //           sel->fds[i].handle_except(i, sel->fds[i].data);
           //       }
           //   }
           // }
        }
    }   
}
int main(int argc, char *argv[])  
{  

    int    fd, c=0, res;  

    char  buf[BUFF_SIZE];  

    printf("Start...\n");  
    fd = open(UART_DEVICE, O_RDWR);  

    if (fd < 0) {  
        perror(UART_DEVICE);  
        exit(1);  
    }  

    printf("Open...\n");  
    set_speed(fd,115200);  
    if (set_Parity(fd,8,1,'N') == FALSE)  {  
        printf("Set Parity Error\n");  
        exit (0);  
    }  

    printf("Reading...\n"); 
    read_nonblock(fd);
//    while(1) 
//    {  
//        res = read(fd, buf, 255);  
//
//        //if(res==0)  
//        //continue;  
//        buf[res]=0;  
//
//        printf("%s", buf);  
//
//        if (buf[0] == 0x0d)  
//        printf("\n");  
//
//        printf("write data to the serial:......\n");
//        char sline[BUFF_SIZE]={0};
//        fgets(sline,BUFF_SIZE,stdin);
//
//        if(write(fd,sline,strlen(sline),0)<0)
//
//        {
//            printf("send msg wrong ....\n");
//            exit(0);
//        }
//
//        if (buf[0] == '@') break;  
//    }  
//
    printf("Close...\n");  
    close(fd);  

    return 0;  
}  
