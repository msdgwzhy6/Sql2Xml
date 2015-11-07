#include <time.h>
#include <string.h>
#include <stdio.h> 
#include <assert.h>

FILE *fp;
/*
    功能：     获取当前系统时间
    返回值：    0-成功，-1-失败
    out:        保存返回的系统时间，格式由fmt决定
    fmt:        0-返回：yyyy-mm-dd hh24:mi:ss, 1-返回：yyyy-mm-dd, 2-返回：hh24:mi:ss
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
    功能：     初始化日志文件
    返回值：    1-成功，0-失败
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
    功能：     将str写入到文件
    返回值：    写文件成功返回0,否则返回-1
    head:        待写入的日志头信息    str:        待写入的字符串
    说明：     写入到日志格式：[2011-04-12 12:10:20][head][str])
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
        return 0;               // 写文件成功
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
        return 0;               // 写文件成功
    }
    else
        return -1;
}

int closeFile()
{
	writeFile("END","File Convert Over!");
	writeLinefeed();   // 文本里换行 
    return fclose(fp);
}
