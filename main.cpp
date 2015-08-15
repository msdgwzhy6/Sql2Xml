#include <iostream>
#include <string>
#include "SqlToXml.h"
#include "SimpleLog.h"

#ifdef WIN32
#include <io.h>
#endif

#ifdef LINUX
#include <unistd.h>
#endif

using namespace  std;

int main(int argc,char *argv[])
{
	if(!openFile())
	{
		cout<<"create log error!!! >"<<endl;
		return -1;
	}
	if(argc<=1)
	{
		writeFile("ERROR","Program Load Error");
		cout<<"Please Input FilePath: < sql2xml *.sql >"<<endl;
		closeFile();
		return -1;
	}
	for(int i=1;i<argc;i++)
	{	
	    string sqlPath;
	    sqlPath = argv[i];
	    
	    // 先判断文件是否存在
	    // int access(const char *pathname, int mode);
	    if(-1==access(sqlPath.c_str(),F_OK))
	    {
	    	writeFile("ERROR",(sqlPath+" File not exists!").c_str());
			continue;
	    }
	    else
	    {
	    	// 如果存在该文件，就判断是否是  .sql 的文件
	    	
	    	//".sql"文件，长度至少是5
			//int compare(int pos, int n,const string &s)const;
			//比较当前字符串从pos开始的n个字符组成的字符串与s的大小
			int size = sqlPath.size();
			if(size<5)
			{
				writeFile("ERROR",(sqlPath+" is not sql file!").c_str());
				continue;
			}
			else if(0!=sqlPath.compare(sqlPath.length()-4,4,".SQL"))
			{
				if(0!=sqlPath.compare(sqlPath.length()-4,4,".sql"))
				{
					writeFile("ERROR",(sqlPath+" is not sql file!").c_str());
					continue;
				}
			}
	    }
	#ifdef linux
 		// 先执行Linux下的sed命令，把打开的文件转为linux下的文件
	    // windows下换行符有:\r\n,Linux下换行符只有:\n,Unix下只有:\r
	    string str_command="sed -i 's/\r//' ";
	    str_command = str_command+sqlPath;
	    system(str_command.c_str());
	#endif

	    SqlToXml sqlToXml;
	    if (!sqlToXml.GetStatementBySql(sqlPath))
	    {
			writeFile("ERROR",(sqlPath+" File Load Error").c_str());
			continue;
	    }
	    sqlToXml.CreateXml();
	   	sqlToXml.clear();
	}
	closeFile();
	return 0;
}
