/*************************************************************************
    > File Name: XORVaild.c
    > Author: leeksir
    > Mail: leek9527@163.com 
    > Created Time: Thu May  3 10:29:49 2018
 ************************************************************************/

#include<stdio.h>
#include<string.h>

void XORValid(const char* buffer,int len)
{
	char checksum = 0;
	char chl, cl;
	int i = 0;
	for(; i<len; i++)
	{
		checksum ^= buffer[i];
	}
	chl = (checksum >> 4) & 0x0F;
	cl = checksum & 0x0F;
	printf("%X\r\n", checksum & 0xFF);
/*
	if(chl < 10)
		chl += '0';
	else
		chl = (chl - 10) + 'A';
	if(cl < 10)
		cl += '0';
	else
		cl = (cl - 10) + 'A';
    char XOR_High = chl;
	char XOR_Low = cl;
	printf("%c%c\r\n",chl,cl);
	*/
}

/*
char doBcc(char *pData, int nLen)
{
	int nPos = 0;
	char cRtBcc = 0;
	do{
		cRtBcc ^= pData[nPos];
		nPos++;
	}while(nPos < nLen);
	return cRtBcc;
}
*/
int main()
{
	char str[6] = {0x40,0x0D,0xFF,0x00,0x00,0x00};
	char buf[10] = "hello";
	printf("strlen(buf) = %d\r\n", strlen(buf));
	//char str[3] = {0x40,0x00,0xFF};
	//printf("%X\r\n",doBcc(str, strlen(str)));
	//printf("%X\r\n", 15);
	XORValid(str, sizeof(str));
	printf("strlen(str) = %d\r\n", strlen(str));
	printf("sizeof(str) = %d\r\n", sizeof(str));
	//XORValid(str, strlen(str));


	return 0;
}
