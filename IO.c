/*************************************************************************
    > File Name: IO.c
    > Author: leeksir
    > Mail: leek9527@163.com 
    > Created Time: Wed May  9 09:55:30 2018
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//打开串口
int open_uart(void)
{
	int fd;
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(-1 == fd)
	{
		perror("can't Open Port!!!");
	}
	else
	{
		printf("Open Port success!!!\r\n");
		close(fd);
	}

	return fd;
}

//
void uart_send(int fd, uchar* cmt, int length)
{

}
//
void uart_recv(int fd, uchar* ret, int length)
{

}

int main()
{
	int fd;
	fd = open_port();
	printf("fd = %d\r\n", fd);
	char str[8] = {0x40,0x0D,0xFF,0x00,0x00,0x00,0xB2,0x0D};
	//char str[10] = {0x00,0x40,0x11,0xFF,0x00,0x00,0x01,0xAF,0x0D,0x01};
	write(fd, str, sizeof(str));
	sleep(3);

	char buf[8];
	read(fd, buf, sizeof(buf));
	int i = 0;
	for(; i<sizeof(buf); i++)
	{
		printf("%x ", buf[i] & 0xff);
	}
	putchar(10);

	return 0;
}
