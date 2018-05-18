/*************************************************************************
    > File Name: vein.c
    > Author: leeksir
    > Mail: leek9527@163.com 
    > Created Time: Wed May  9 11:00:17 2018
 ************************************************************************/

#include<stdio.h>

#define ALL_VEIN -1
typedef unsigned char uchar;

//vein check touch/notouch
int vein_check()
{
	//检测手指状态
	write(fd, buf, )
		
	return 0;
}

//recognition vein
int vein_recognition()
{
	//进行比对
	return 0;
}

//add vein
int vein_type()
{
	static int fidok
	return 0;
}

//delete vein
int vein_delete(int fid)
{
	if(ALL_VEIN == fid)
	{
		//删除所有的指静脉
	}
	else
	{
		//根据fid删除指静脉
	}
	return 0;
}

//check_sum 
uchar check_sum(uchar *buffer,int len)
{
	uchar checksum = 0;
	int i = 0;
	for(; i<len; i++)
	{
		checksum ^= buffer[i];
	}
	return checksum & 0xFF;
}

//USB data
void init_USB()
{
	
}

int main()
{
	
	uchar str[6] = {0x40,0x0D,0xFF,0x00,0x00,0x00};
	uchar buf[10] = "hello";
	printf("strlen(buf) = %d\r\n", strlen(buf));
	//char str[3] = {0x40,0x00,0xFF};
	//printf("%X\r\n",doBcc(str, strlen(str)));
	//printf("%X\r\n", 15);
	printf("%X\r\n",check_sum(str, sizeof(str)));
	printf("strlen(str) = %d\r\n", strlen(str));
	printf("sizeof(str) = %d\r\n", sizeof(str));

	return 0;
}

