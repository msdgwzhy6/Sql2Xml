#include <time.h>
#include <string.h>
#include <stdio.h> 
#include <assert.h>

FILE *fp;
/*
    ���ܣ�     ��ȡ��ǰϵͳʱ��
    ����ֵ��    0-�ɹ���-1-ʧ��
    out:        ���淵�ص�ϵͳʱ�䣬��ʽ��fmt����
    fmt:        0-���أ�yyyy-mm-dd hh24:mi:ss, 1-���أ�yyyy-mm-dd, 2-���أ�hh24:mi:ss
*/
int getTime(char *out, int fmt)
{
    if(out == NULL)
        return -1;
 
    time_t t;
    struct tm *tp;
    t = time(NULL);
 
    tp = localtime(&t);
    if(fmt == 0)
        sprintf(out, "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
    else if(fmt == 1)
        sprintf(out, "%2.2d-%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
    else if(fmt == 2)
        sprintf(out, "%2.2d:%2.2d:%2.2d", tp->tm_hour, tp->tm_min, tp->tm_sec);
    return 0;
}
/*
    ���ܣ�     ��ʼ����־�ļ�
    ����ֵ��    1-�ɹ���0-ʧ��
*/ 
int openFile()
{
    char curtime[100]={0};
    getTime(curtime,1);
    int len = strlen(curtime);
    if(len+4<100)
    {
	curtime[len]='.';
	curtime[len+1]='l';
	curtime[len+2]='o';
	curtime[len+3]='g';
	curtime[len+4]='\0';
    }
    fp = fopen(curtime, "a");
    if(fp==NULL)return 0;
    return 1;
}

/*
    ���ܣ�     ��strд�뵽�ļ�
    ����ֵ��    д�ļ��ɹ�����0,���򷵻�-1
    head:        ��д�����־ͷ��Ϣ    str:        ��д����ַ���
    ˵����     д�뵽��־��ʽ��[2011-04-12 12:10:20][head][str])
*/
int writeFile(const char *head,const char *str)
{
    assert(fp != NULL && str != NULL);
    char curTime[100] = {0};
    int ret = -1;
    getTime(curTime, 0);
    ret = fprintf(fp, "[%s][%s] %s\n", curTime,head,str);
    
    if(ret >= 0)
    {
	printf("[%s][%s] %s\n",curTime,head,str);
        fflush(fp);
        return 0;               // д�ļ��ɹ�
    }
    else
        return -1;
}

int writeLinefeed()
{
    int ret = -1;
    ret = fprintf(fp, "\n");
    
    if(ret >= 0)
    {
        fflush(fp);
        return 0;               // д�ļ��ɹ�
    }
    else
        return -1;
}

int closeFile()
{
	writeFile("END","File Convert Over!");
	writeLinefeed();   // �ı��ﻻ�� 
    return fclose(fp);
}
