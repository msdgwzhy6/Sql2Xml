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
	    
	    // ���ж��ļ��Ƿ����
	    // int access(const char *pathname, int mode);
	    if(-1==access(sqlPath.c_str(),F_OK))
	    {
	    	writeFile("ERROR",(sqlPath+" File not exists!").c_str());
			continue;
	    }
	    else
	    {
	    	// ������ڸ��ļ������ж��Ƿ���  .sql ���ļ�
	    	
	    	//".sql"�ļ�������������5
			//int compare(int pos, int n,const string &s)const;
			//�Ƚϵ�ǰ�ַ�����pos��ʼ��n���ַ���ɵ��ַ�����s�Ĵ�С
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
 		// ��ִ��Linux�µ�sed����Ѵ򿪵��ļ�תΪlinux�µ��ļ�
	    // windows�»��з���:\r\n,Linux�»��з�ֻ��:\n,Unix��ֻ��:\r
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
